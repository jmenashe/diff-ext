# Copyright (c) 2008-2009 Sergey Zorin
# This software is distributable under the BSD license. See the terms
# of the BSD license in the COPYING file provided with this software.

Summary: Konqueror context menu extension to launch file comparison tool

Name: 		kdiff-ext
Version: 	0.4.0
Release: 	1
License: 	BSD
Group: 		User Interface/Desktops 
Source: 	http://downloads.sourceforge.net/diff-ext/kdiff-ext-%{version}.tar.gz 
URL: 		http://diff-ext.sourceforge.net
BuildRoot: 	%{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildRequires:	kdebase4-devel >= 4.1, kdelibs-devel >= 4.1, gcc-c++, cmake >= 2.6, gettext

%description
This Konqueror extension (kdiff-ext) makes it possible to launch file 
comparison tool for 2 or 3 files (depending on the tool) in the same directory
or "save" a file(s) for later comparison. It acts as external most recent used 
(MRU) file list for the comparison tool.

%prep
%setup -q -n %{name}-%{version}

%build
%configure
%__make %{?_smp_mflags}

%install
[ -d %{buildroot} -a "%{buildroot}" != "" ] && %__rm -rf  %{buildroot}
%makeinstall

%clean
[ -d %{buildroot} -a "%{buildroot}" != "" ] && %__rm -rf  %{buildroot}

%files
%{_bindir}/kdiffextsetup
%{_libdir}/kde4/kdiff_ext.la
%{_libdir}/kde4/kdiff_ext.so
%{_datadir}/applications/kde4/kdiffextsetup.desktop 
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
%{_datadir}/kde4/services/kdiff-ext.desktop
%{_datadir}/locale/pt_BR/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/pt/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/sv/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/gl/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/pl/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/fr/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/he/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/ru/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/de/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/ko/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/es/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/da/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/nb/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/uk/LC_MESSAGES/%{name}.mo
%{_datadir}/locale/it/LC_MESSAGES/%{name}.mo

%changelog
* Mon Feb 2 2009 Sergey Zorin - 0.4.0
- port to KDE 4.1+
