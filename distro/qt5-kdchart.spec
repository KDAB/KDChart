Name:           qt5-kdchart
Version:        3.0.0
Release:        1
Summary:        A Qt tool for creating business and scientific charts
Source0:        %{name}-%{version}.tar.gz
Source1:        %{name}-rpmlintrc
Source2:        %{name}-%{version}.tar.gz.asc
URL:            https://github.com/KDAB/KDChart
Group:          System/Libraries
License:        MIT
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Vendor:         Klaralvdalens Datakonsult AB (KDAB)
Packager:       Klaralvdalens Datakonsult AB (KDAB) <info@kdab.com>

BuildRequires: cmake
%if %{defined suse_version}
BuildRequires:  libqt5-qtbase-devel libqt5-qtsvg-devel
%endif

%if %{defined fedora}
BuildRequires:  gcc-c++ qt5-qtbase-devel qt5-qtsvg-devel desktop-file-utils
%endif

%if %{defined rhel}
BuildRequires:  gcc-c++ qt5-qtbase-devel qt5-qtsvg-devel desktop-file-utils
%endif

%description
KDAB's KD Chart a tool for creating business and scientific charts.
It provides developers with a sophisticated way to customize layouts and
to design and manage large numbers of axes. KD Chart targets C++ programmers
who use Qt in their applications.

Authors:
--------
      Klaralvdalens Datakonsult AB (KDAB) <info@kdab.com>

%define debug_package %{nil}
%global __debug_install_post %{nil}

%package devel
Summary:        Development files for %{name}
Group:          Development/Libraries/C and C++
Requires:       %{name} = %{version}

%description devel
This package contains header files and associated tools and libraries to
develop programs using kdchart.

%prep
%autosetup

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
%__make %{?_smp_mflags}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%install
%make_install

%clean
%__rm -rf "%{buildroot}"

%files
%defattr(-,root,root)
%{_prefix}/share/doc/KDChart
%{_libdir}/libkdchart.so.*

%files devel
%defattr(-,root,root,-)
%{_libdir}/qt5/mkspecs/modules/*
%dir %{_prefix}/share/mkspecs
%dir %{_prefix}/share/mkspecs/features
%{_prefix}/share/mkspecs/features/kdchart.prf
%dir %{_includedir}/KDChart
%{_includedir}/KDChart/*
%dir %{_libdir}/cmake/KDChart
%{_libdir}/cmake/KDChart/*
%{_libdir}/libkdchart.so

%changelog
* Sat Aug 27 2022 Allen Winter <allen.winter@kdab.com> 3.0.0
  3.0.0
* Fri Oct 16 2020 Allen Winter <allen.winter@kdab.com> 2.7.2
  2.7.2
* Fri Jun 19 2020 Allen Winter <allen.winter@kdab.com> 2.7.1
  2.7.1
* Mon Jan 06 2020 Allen Winter <allen.winter@kdab.com> 2.7.0
  2.7.0
* Mon Apr 22 2019 Allen Winter <allen.winter@kdab.com> 2.6.2
  2.6.2 final
