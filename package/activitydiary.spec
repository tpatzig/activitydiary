Name:           activitydiary
BuildRequires:  libkde4-devel
BuildRequires:  libqwt5-devel
BuildRequires:  cmake >= 2.4.6
License:        GPL
Group:          System/Management
Summary:        ad
Version:        0.1
Release:        1
URL:            http://en.opensuse.org/
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Source0:        %{name}-%{version}.tar.bz2
Requires:       libqwt5


%description
A Diary for recorded GPS-Tracks.

Authors:
--------
Thomas Goettlicher <tgoettlicher@suse.de>
Tom Patzig <tpatzig@suse.de>


%prep
%setup -q -n %{name}-%{version} 

%build
qmake activitydiary.pro
%make_jobs DESTDIR=%buildroot%_prefix

%install
mkdir -p %buildroot%_prefix/bin
%makeinstall INSTALL_ROOT=%{buildroot} 
#%kde_post_install



%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%_prefix/share/activitydiary
/usr/share/icons/hicolor/*/apps/activitydiary.*
/usr/share/applications/kde4/activitydiary.desktop
%_prefix/bin/activitydiary


%changelog

=======
Name:           activitydiary    
BuildRequires:  libkde4-devel    
BuildRequires:  libqwt5-devel    
BuildRequires:  libqwt5    
#BuildRequires: -post-build-checks      
License:        GPL    
Group:          System/Management    
Summary:        activitydiary    
Version:        0.1    
Release:        1    
URL:            http://en.opensuse.org/    
BuildRoot:      %{_tmppath}/%{name}-%{version}-build    
Source0:        %{name}.tar.bz2    
# tar cjf  activitydiary.tar.bz2  ../../activitydiary/  
Requires:       libqwt5    
  
  
   
%description    
activitydiary    
   
Authors:    
--------    
Tom Patzig <tpatzig@suse.de>    
Thomas Goettlicher <tgoettlicher@suse.de>    
  
%prep    
#%setup -q -n %{name}-%{version}     
%setup -q -n %{name}    
   
%build    
qmake activitydiary.pro    
make    
   
%install    
make install INSTALL_ROOT=%{buildroot}  
   
   
%kde_post_install    
   
   
%clean    
rm -rf $RPM_BUILD_ROOT    
   
%files    
%defattr(-,root,root)    
   
/usr/bin/activitydiary  
/usr/share/activitydiary/icons  
/usr/share/applications/kde4/activitydiary.desktop
/usr/share/icons/hicolor/128x128/apps/activitydiary.png
/usr/share/icons/hicolor/32x32/apps/activitydiary.png
/usr/share/icons/hicolor/48x48/apps/activitydiary.png
/usr/share/icons/hicolor/64x64/apps/activitydiary.png
/usr/share/activitydiary

#/opt/activitydiary/icons/cycling.png    
#/opt/activitydiary/icons/end.gif    
#/opt/activitydiary/icons/hiking.png    
#/opt/activitydiary/icons/kompassberg.png    
#/opt/activitydiary/icons/kompassberg_small.png    
#/opt/activitydiary/icons/loading.png    
#/opt/activitydiary/icons/running.png    
#/opt/activitydiary/icons/start.gif    
#/opt/activitydiary     
   
  
%changelog    
  
