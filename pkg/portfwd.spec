%define name portfwd
%define version 0.26
%define release 1

Summary: portfw TCP/UDP forwarding
Name: %{name}
Version: %{version}
Release: %{release}
Source0: %{name}-%{version}.tar.gz
Copyright: GPL
Group: System Environment/Daemons
BuildRoot: %{_tmppath}/%{name}-buildroot
Prefix: %{_prefix}
URL: http://portfwd.sf.net


%description

Portfwd stands for port forwarding daemon. It's a small C++
utility which forwards incoming TCP connections and/or UDP
packets to remote hosts.

%prep

case "${RPM_COMMAND:-all}" in
all)
%setup -q
;;esac

%build

case "${RPM_COMMAND:-all}" in
all)
export CFLAGS="$RPM_OPT_FLAGS -O1 -g"

./bootstrap
./configure --prefix=%{prefix} --sysconfdir=/etc
;;esac

make 

%install
install -d $RPM_BUILD_ROOT%{prefix}/bin
install -d $RPM_BUILD_ROOT%{prefix}/man/man1
make prefix=$RPM_BUILD_ROOT%{prefix} install 

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc COPYING README TODO cfg
%{_prefix}/
#%#(config)/etc/*
