Name: org.tizen.favorite
Summary: Favorite application for Tizen TV based on AirFlex UX
Version: 0.2
Release: 1
Group: Applications/Core Applications
License: Apache-2.0
Source0: %{name}-%{version}.tar.gz
Source1: %{name}.manifest

BuildRequires: cmake
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(app-utils)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(capi-content-media-content)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(capi-web-bookmark)
BuildRequires: pkgconfig(tv-service)
BuildRequires: pkgconfig(ui-gadget-1)

%define _appdir /usr/apps/%{name}
%define _bindir %{_appdir}/bin
%define _resdir %{_appdir}/res
%define _edjedir %{_resdir}/edje
%define _imgdir %{_resdir}/images
%define _pkgdir %{_datadir}/packages
%define _desktop_icondir /usr/share/icons

%description
Favorite application shows the contents which were registered as favorite.

%prep
%setup -q
cp %{SOURCE1} .

%build
cmake \
	-DCMAKE_INSTALL_PREFIX=%{_appdir} \
	-DPACKAGE_NAME=%{name} \
	-DVERSION=%{version} \
	-DBINDIR=%{_bindir} \
	-DRESDIR=%{_resdir} \
	-DEDJEDIR=%{_edjedir} \
	-DIMGDIR=%{_imgdir} \
	-DDESKTOP_ICONDIR=%{_desktop_icondir} \
	-DDESKTOP_ICON=%{name}.png \
	-DPACKAGEDIR=%{_pkgdir}

make %{?jobs:-j%jobs}

# Copies the executables to appropriate location
%install
%make_install

%clean
rm -rf %{buildroot}

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_bindir}/*
%{_resdir}/*
%{_pkgdir}/%{name}.xml
%{_desktop_icondir}/%{name}.png
