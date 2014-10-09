Name: handystats
Version: 1.10.2
Release: 1%{?dist}
Summary: C++ library for collecting user-defined in-process runtime statistics with low overhead.
Group: System Environment/Libraries
License: GPLv2+
URL: https://github.com/shindo/handystats
Source0: %{name}-%{version}.tar.bz2
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%if %{defined rhel} && 0%{?rhel} < 7
BuildRequires: cmake28
%else
BuildRequires: cmake
%endif

BuildRequires: boost-devel
BuildRequires: gtest-devel

%description
C++ library for collecting user-defined in-process runtime statistics with low overhead.

%prep
%setup -q

%build
mkdir -p %{_target_platform}
pushd %{_target_platform}

%if %{defined rhel} && 0%{?rhel} < 7
%{cmake28} ..
%else
%{cmake} ..
%endif

popd

make %{?_smp_mflags} -C %{_target_platform}

%check
make check -C %{_target_platform}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot} -C %{_target_platform}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_includedir}/handystats/*
%{_libdir}/*handystats*.so*

#%changelog
