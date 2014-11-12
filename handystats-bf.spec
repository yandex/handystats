# library soname
%global __soname 2

# python things
%if 0%{?rhel} && 0%{?rhel} <= 6
%{!?__python2: %global __python2 /usr/bin/python2}
%{!?python2_sitelib: %global python2_sitelib %(%{__python2} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())")}
%{!?python2_sitearch: %global python2_sitearch %(%{__python2} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%endif


Name: handystats
Version: 2.0.0
Release: 1%{?dist}
Summary: C++ library to collect runtime statistics
License: LGPLv3+
URL: https://github.com/shindo/handystats
Source: https://github.com/shindo/handystats/archive/%{version}.tar.gz
BuildRoot: %{_tmppath}/handystats-%{version}-root

# Build dependencies
BuildRequires: boost-devel
BuildRequires: gtest-devel
BuildRequires: python-devel

# CMake workaround
%if 0%{?rhel} && 0%{?rhel} <= 6
BuildRequires: cmake28
%global __cmake %{cmake28}
%else
BuildRequires: cmake
%global __cmake %{cmake}
%endif

%description
C++ library for collecting user-defined
in-process runtime statistics with low overhead.


%prep
%setup -q
#%%prep


%build
# libhandystats
cd %{_builddir}/%{name}-%{version}
%{__cmake} -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo -DWITH_TESTS=ON -DWITH_PYTHON=OFF -DWITH_UTILS=OFF -DWITH_BENCHMARKS=OFF
make %{?_smp_mflags} -j $(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1)

# python bindings
cd %{_builddir}/%{name}-%{version}/bindings/python
%{__python2} setup.py build_py \
	--force \
	--verbose
%{__python2} setup.py build_ext \
	--include-dirs "%{_builddir}/%{name}-%{version}/include" \
	--library-dirs "%{_builddir}/%{name}-%{version}/lib:%{_builddir}/%{name}-%{version}/lib64" \
	--force \
	--verbose

# utils
cd %{_builddir}/%{name}-%{version}/utils
%{__cmake} -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo
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

# python bindings
cd %{_builddir}/%{name}-%{version}/bindings/python
%{__python2} setup.py install \
	--root="%{buildroot}/" \
	--optimize=1 \
	--skip-build \
	--verbose

# utils
cd %{_builddir}/%{name}-%{version}/utils
%make_install

#%%install



# libhandystats binary package
%package -n libhandystats%{__soname}
Summary: C++ library to collect runtime statistics - Core library
Group: Development/Libraries
Obsoletes: handystats < 2.0.0

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
Obsoletes: handystats < 2.0.0

%description -n libhandystats-devel
Handystats is C++ library for collecting user-defined
in-process runtime statistics with low overhead.

%files -n libhandystats-devel
%defattr(-,root,root,-)
%{_includedir}/handystats/
%{_libdir}/libhandystats.so


# python-handystats binary package
%package -n python-handystats
Summary: Python bindings for libhandystats%{__soname}
Group: Development/Languages
Requires: libhandystats%{__soname} = %{version}-%{release}
Requires: python

%description -n python-handystats
Handystats is C++ library for collecting user-defined
in-process runtime statistics with low overhead.

%files -n python-handystats
%defattr(-,root,root,-)
%{python2_sitearch}/handystats/
%{python2_sitearch}/*.egg-info


# handystats-utils binary package
%package -n handystats-utils
Summary: C++ library to collect runtime statistics - Utilities
Group: Development/Tools
Requires: libhandystats%{__soname} = %{version}-%{release}
Requires: python-handystats = %{version}-%{release}
Requires: python

%description -n handystats-utils
Handystats is C++ library for collecting user-defined
in-process runtime statistics with low overhead.

%files -n handystats-utils
%defattr(-,root,root,-)
%{_bindir}/*



%clean
rm -rf %{buildroot}
#%%clean


%changelog
* Sun Dec 28 2014 Danil Osherov <shindo@yandex-team.ru> 2.0.0

- added template methods for writing handystats objects to rapidjson::Value.

- read/write time unit with handystats::chrono::time_unit_from_string(),
  handystats::chrono::time_unit_as_string().

- added support for serialization/deserialization statistical data with
  JSON text and rapidjson::Value objects.

- added experimental file logger backend.

- added aggregation mechanics for statistical data.

- added aggregate_log utility.

- added python bindings for aggregation mechanics.

- added new statistics:
  - throughput -- moving sum of data values averaged to second
  - frequency -- moving count of data values averaged to second

- added HANDY_CONFIG_ERROR() method to access error message occurred
  during configuration application.

- added support for CLOCK_MONOTONIC_RAW clock source.

- TSC rate determination fixed which may lead to tests failure due to
  TSC rate inaccuracy.

- number of global objects radically reduced.
  During program termination without calling HANDY_FINALIZE()
  order-of-destruction problem may occur and lead to abnormal termination.

- configuration methods HANDY_CONFIG_JSON() and HANDY_CONFIG_FILE() don't
  throw exceptions.

- configuration format simplified:
  - statistics -> defaults
  - core.enable -> enable
  - tags -> stats
  - metrics-dump.interval -> dump-interval
  - metrics section removed
  - gauge, counter, timer sections moved to the top level

- added pattern statistics configuration.

- common map of metrics in dump split into two maps -- metrics and
  attributes.

- metrics objects replaced with corresponding statistics objects in dump.

- removed rapidjson headers from public library headers.

- removed support for rapidjson::Value in HANDY_CONFIG_JSON() method.

- attribute handystats.dump_timestamp in dump replaced with:
  - handystats.system_timestamp represents unix timestamp in milliseconds
    to which current dump is related
  - handystats.internal_timestamp represents unix timestamp in milliseconds
    to which user metrics' state is related

- removed rate statistic.

- removed deprecated statistic extraction interface (min(), max(), etc.).
