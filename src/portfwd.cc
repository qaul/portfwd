/*
  portfwd.c

  $Id: portfwd.cc,v 1.5 2002/04/20 04:32:35 evertonm Exp $
 */


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <stdlib.h>

#include "getopt.h"
#include "portfwd.h"
#include "util.h"
#include "vector.hpp"
#include "iterator.hpp"
#include "entry.hpp"
#include "config.h"

extern FILE           *yyin;
extern int            yyparse();
extern int            conf_syntax_errors;
extern vector<entry*> *entry_vector;

const int BUF_SZ = 8192;
const char * const portfwd_version = VERSION;

#ifdef HAVE_MSG_PROXY
int transparent_proxy = 0;
#endif 

int on_the_fly_dns = 0;
int foreground = 0;

void usage(FILE *out) 
{
  const char *prog = get_prog_name();
  fprintf(out, "Usage: %s [<options>]\n"
	       "       -h               | --help\n"
	       "       -v               | --version\n"
	       "       -d               | --debug\n"

#ifdef HAVE_MSG_PROXY
               "       -t               | --transparent-proxy\n"
#endif

	       "       -f               | --on-the-fly-dns\n"
	       "       -g               | --foreground\n"
	       "       -c <config-file> | --config <config-file>\n",
	  prog);
}

void show_version(FILE *out) 
{
  fprintf(out, "%s %s\n", get_prog_name(), portfwd_version);
}

#
void parse_cmdline(int argc, const char *argv[], const char **config)
{
  const char * me = argv[0];
  const char * const DEFAULT_CONFIG = PORTFWD_CONF;

  int opt;
  int option_index = 0;
  struct option long_options[] =
  {
    {"help", 0, 0, 'h'},
    {"version", 0, 0, 'v'},
    {"debug", 0, 0, 'd'},

#ifdef HAVE_MSG_PROXY
    {"transparent-proxy", 0, 0, 't'},
#endif 

    {"on-the-fly-dns", 0, 0, 'f'},
    {"foreground", 0, 0, 'g'},
    {"config", 1, 0, 'c'},
    {0, 0, 0, 0}
  };

  *config  = 0;

  for (;;) {
		 
    opt = getopt_long(argc, (char ** const) argv, "hvdtfgc:", long_options, &option_index);
    if (opt == -1)
      break;

    switch (opt) {
    case 'h':
      usage(stdout);
      exit(0);
      break;
    case 'v':
      show_version(stdout);
      exit(0);
      break;
    case 'd':
      ++verbose_mode;
      break;

#ifdef HAVE_MSG_PROXY
    case 't':
      ++transparent_proxy;
      break;
#endif

    case 'f':
      ++on_the_fly_dns;
      break;
    case 'g':
      ++foreground;
      break;
    case 'c':
      if (*config) {
	fprintf(stderr, "%s: Configuration file redefinition: %s", me, optarg);
	exit(1);
      }
      *config = optarg;
      break;
    case '?':
      usage(stderr);
      exit(1);
      break;
    default:
      fprintf(stderr, "%s: getopt() returned unknown option: %c (%d)", me, opt, opt);
      exit(1);
    }
  }

  if (optind < argc) {
    fprintf(stderr, "%s: Invalid argument: %s", me, argv[optind]);
    exit(1);
  }

  if (!*config)
    *config = DEFAULT_CONFIG;
}

void read_config(const char *cfg)
{
  ONVERBOSE(syslog(LOG_INFO, "Configuration file: '%s'", cfg));

  yyin = fopen(cfg, "r");
  if (!yyin) {
    syslog(LOG_ERR, "Can't open configuration file: '%s': %m", cfg);
    exit(1);
  }

  yyparse();
  
  if (conf_syntax_errors) {
    syslog(LOG_ERR, "Syntax errors: %d", conf_syntax_errors);
    exit(1);
  }

  if (fclose(yyin)) {
    syslog(LOG_ERR, "Can't close configuration file: '%s'", cfg);
    exit(1);
  }
}

void do_show(vector<entry*>* entries)
{
  if (!entries) {
    syslog(LOG_ERR, "Internal error: Null entry vector (!)");
    return;
  }

  if (!entries->get_size()) {
    syslog(LOG_INFO, "/* Empty */");
    return;
  }

  iterator<vector<entry*>,entry*> it(*entries);
  for (it.start(); it.cont(); it.next())
    it.get()->show();
}


void do_forward(vector<entry*>* entries)
{
  if (!entries) {
    syslog(LOG_ERR, "Internal error: Null entry vector (!)");
    return;
  }

  if (!entries->get_size()) {
    syslog(LOG_NOTICE, "Nothing to do: empty configuration file");
    return;
  }

  iterator<vector<entry*>,entry*> it(*entries);
  for (it.start(); it.cont(); it.next())
    it.get()->serve();
}

void child_reaper(int sig)
{
  int status;
  void (*prev_handler)(int);

  /* pid_t child_pid = waitpid(-1, &status, WNOHANG); */
  pid_t child_pid = wait(&status);
  prev_handler = signal(SIGCHLD, child_reaper);
  if (prev_handler == SIG_ERR) {
    syslog(LOG_ERR, "signal() failed on child_reaper re-install: %m");
    return;
  }

  if (child_pid == -1) {
    syslog(LOG_ERR, "Error waiting pid of child: %m");
    return;
  }
  
  if (WIFEXITED(status)) {
    syslog(LOG_WARNING, "Child with PID %d exited normally with exit status: %d", child_pid, WEXITSTATUS(status));
    return;
  }

  if (WIFSIGNALED(status))
    syslog(LOG_NOTICE, "Child received signal: %d", WTERMSIG(status)); 
    
  syslog(LOG_ERR, "Child with PID %d exited abnormally", child_pid);
}

void term_handler(int sig)
{
  void (*prev_handler)(int);

  prev_handler = signal(SIGTERM, SIG_IGN);
  if (prev_handler == SIG_ERR)
    syslog(LOG_ERR, "signal() failed on term_handler de-install: %m");

  if (kill(0, SIGTERM))
    syslog(LOG_ERR, "Can't terminate children");

  syslog(LOG_INFO, "SIGTERM - Program terminated");

  closelog();

  exit(0);
}

int main(int argc, const char *argv[]) 
{
  set_prog_name("portfwd");

  /*
   * Read arguments from command line.
   */
  const char *config;
  parse_cmdline(argc, argv, &config);

  /*
   * Close file descriptors.
   */
  close_fds();

  /*
   * Connect standard IO to /dev/null.
   */
  std_to_null(); 

  /*
   * Open interface to syslog.
   */
  openlog(get_prog_name(), LOG_CONS | LOG_PID, LOG_DAEMON);
  syslog(LOG_INFO, "%s %s started", get_prog_name(), portfwd_version);

  ONVERBOSE(syslog(LOG_DEBUG, "Verbose mode: %d", verbose_mode));

#ifdef HAVE_MSG_PROXY
  ONVERBOSE(syslog(LOG_INFO, "Transparent proxy mode: %s (%d)", transparent_proxy ? "on" : "off", transparent_proxy));
#else
  ONVERBOSE(syslog(LOG_INFO, "Transparent proxy mode: disabled on compile time"));
#endif

  ONVERBOSE(syslog(LOG_INFO, "On the fly DNS mode: %s (%d)", on_the_fly_dns ? "on" : "off", on_the_fly_dns));

  ONVERBOSE(syslog(LOG_INFO, "Foreground: %s (%d)", foreground ? "on" : "off", foreground));

  /*
   * Go to background.
   */
  if (!foreground) {
    int dmz = daemonize();
    if (dmz) {
      syslog(LOG_ERR, "daemonize() failed: %d", dmz);
      exit(1);
    }
  }

  /*
   * Connect standard IO to /dev/null.
   */
  /* std_to_null(); */

  solve_protonumbers();

  /*
   * Load configuration.
   */
  read_config(config);
  ONVERBOSE(do_show(entry_vector));

  /*
   * Go to root (/) directory to prevent disturbing umount.
   */
  cd_root();

  /*
   * Install handler to take care of children.
   */
  void (*prev_handler)(int);
  prev_handler = signal(SIGCHLD, child_reaper);
  if (prev_handler == SIG_ERR) {
    syslog(LOG_ERR, "signal() failed on child_reaper install: %m");
    exit(1);
  }

  /*
   * Spawn forwarders.
   */
  do_forward(entry_vector);

  /*
   * Install handler for TERM signal.
   */
  prev_handler = signal(SIGTERM, term_handler);
  if (prev_handler == SIG_ERR) {
    syslog(LOG_ERR, "signal() failed on term_handler install: %m");
    exit(1);
  }

  /*
   * Wait forever.
   */
  for (;;)
    sleep(3600);

  return 0;
}
