Name: handystats
Version: 2.0.0
Release: 1%{?dist}
Summary: C++ library to collect runtime statistics
License: LGPLv3+
URL: https://github.com/shindo/handystats
Source: https://github.com/shindo/handystats/archive/%{version}.tar.gz
BuildRoot: %{_tmppath}/handystats-%{version}-root

%description
C++ library for collecting user-defined
in-process runtime statistics with low overhead.

# Build dependencies
BuildRequires: boost-devel
BuildRequires: gtest-devel

# CMake workaround
%if 0%{?rhel} && 0%{?rhel} <= 6
BuildRequires: cmake28
%global __cmake %{cmake28}
%else
BuildRequires: cmake
%global __cmake %{cmake}
%endif

# library soname
%global __soname 2


%prep
%setup -q -c -n %{name}-%{version}
#%%prep

%build
# libhandystats
cd %{_builddir}/%{name}-%{version}
%{__cmake} -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo -DWITH_TESTS=ON
make %{?_smp_mflags} -j $(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1)

#%%build


%check
cd %{_builddir}/%{name}-%{version}
make -k -j $(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1) check
#%%check


%install
rm -rf %{buildroot}

# libhandystats
cd %{_builddir}/%{name}-%{version}
%make_install

#%%install



# libhandystats binary package
%package -n libhandystats%{__soname}
Summary: C++ library to collect runtime statistics - Core library
Group: Development/Libraries

%description -n libhandystats%{__soname}
Handystats is C++ library for collecting user-defined
in-process runtime statistics with low overhead.

%files -n libhandystats%{__soname}
%defattr(-,root,root,-)
%{_libdir}/libhandystats.so.*

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig


# libhandystats-devel binary package
%package -n libhandystats-devel
Summary: C++ library to collect runtime statistics - Development files
Group: Development/Libraries
Requires: libhandystats%{__soname} = %{version}-%{release}
Requires: boost-devel

%description -n libhandystats-devel
Handystats is C++ library for collecting user-defined
in-process runtime statistics with low overhead.

%files -n libhandystats-devel
%defattr(-,root,root,-)
%{_includedir}/handystats/
%{_libdir}/libhandystats.so



%clean
rm -rf %{buildroot}
#%%clean


#%changelog
