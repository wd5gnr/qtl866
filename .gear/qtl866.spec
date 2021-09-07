%define _unpackaged_files_terminate_build 1

Name: qtl866
Version: 0.3.0
Release: alt1

Summary: GUI driver for minipro EPROM/Device programmer software
License: %gpl3plus
Group: Other
Url: https://github.com/wd5gnr/qtl866
Source0: %name-%version.tar

BuildRequires(pre): rpm-build-licenses
BuildRequires(pre): rpm-macros-qt5
BuildRequires: qt5-base-devel

%description
qtl866 - GUI driver for minipro EPROM/Device programmer software

%prep
%setup -q

%build
%_qt5_qmake
%install
%make_install INSTALL_ROOT=%buildroot install
%files
%_bindir/qtl866

%changelog
* Tue Aug 16 2021 Aleksey Saprunov <sa@altlinux.org> 0.3.0-alt1
- Initial release
