# Copyright (c) 2008 Sergey Zorin
# This software is distributable under the BSD license. See the terms
# of the BSD license in the COPYING file provided with this software.

Summary: Konqueror context menu extension to launch file comparison tool

Name: 		kdiff-ext
Version: 	0.3.1
Release: 	1
License: 	BSD
Group: 		User Interface/Desktops 
Source: 	http://downloads.sourceforge.net/diff-ext/kdiff-ext-%{version}.tar.gz 
URL: 		http://diff-ext.sourceforge.net
BuildRoot: 	/var/tmp/%{name}-buildroot
BuildRequires:	kdebase3-devel >= 3.2, kdelibs-devel >= 3.2, gcc-c++, autoconf, automake, gettext

%description
This Konqueror extension (kdiff-ext) makes it possible to launch file 
comparison tool for 2 or 3 files (depending on the tool) in the same directory
or "save" a file(s) for later comparison. It acts as external most recent used 
(MRU) file list for the comparison tool.

%prep
%setup -q -n %{name}-%{version}

%build
./configure --prefix=$RPM_BUILD_ROOT/usr
%__make %{?_smp_mflags}

%install
[ -d %{buildroot} -a "%{buildroot}" != "" ] && %__rm -rf  %{buildroot}
%makeinstall

%clean
[ -d %{buildroot} -a "%{buildroot}" != "" ] && %__rm -rf  %{buildroot}

%files
%{_bindir}/kdiffextsetup
%{_libdir}/kde3/kdiff-ext.la
%{_libdir}/kde3/kdiff-ext.so
%{_datadir}/applnk/Utilities/kdiffextsetup.desktop 
%{_datadir}/apps/kdiffextsetup/kdiffextsetupui.rc 
%{_datadir}/config.kcfg/kdiffext.kcfg
%doc %{_datadir}/doc/HTML/en/kdiffext/common
%doc %{_datadir}/doc/HTML/en/kdiffext/index.cache.bz2
%doc %{_datadir}/doc/HTML/en/kdiffext/index.docbook
%doc %{_datadir}/doc/kdiff-ext/AUTHORS
%doc %{_datadir}/doc/kdiff-ext/COPYING
%doc %{_datadir}/doc/kdiff-ext/ChangeLog
%doc %{_datadir}/doc/kdiff-ext/INSTALL
%doc %{_datadir}/doc/kdiff-ext/NEWS
%doc %{_datadir}/doc/kdiff-ext/README
%doc %{_datadir}/doc/kdiff-ext/TODO
%{_datadir}/icons/hicolor/16x16/actions/clear.png
%{_datadir}/icons/hicolor/16x16/actions/diff.png
%{_datadir}/icons/hicolor/16x16/actions/diff3.png
%{_datadir}/icons/hicolor/16x16/actions/diff3_with.png
%{_datadir}/icons/hicolor/16x16/actions/diff_later.png
%{_datadir}/icons/hicolor/16x16/actions/diff_with.png
%{_datadir}/icons/hicolor/16x16/apps/kdiffextsetup.png
%{_datadir}/icons/hicolor/32x32/apps/kdiffextsetup.png
%{_datadir}/locale/ru/LC_MESSAGES/%{name}.mo
%{_datadir}/services/kdiff-ext.desktop

%changelog

