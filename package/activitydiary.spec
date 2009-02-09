Name:           activitydiary  
BuildRequires:  libkde4-devel  
BuildRequires:  libqwt5-devel  
BuildRequires: -post-build-checks    
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
mkdir /opt/activitydiary/  
cp activitydiary /opt/activitydiary/  
cp -r icons /opt/activitydiary/  
   
%kde_post_install  
  
   
%clean  
rm -rf $RPM_BUILD_ROOT  
   
%files  
%defattr(-,root,root)  
   
/opt/activitydiary/activitydiary  
/opt/activitydiary/icons/calendar.png  
/opt/activitydiary/icons/cycling.png  
/opt/activitydiary/icons/end.gif  
/opt/activitydiary/icons/hiking.png  
/opt/activitydiary/icons/kompassberg.png  
/opt/activitydiary/icons/kompassberg_small.png  
/opt/activitydiary/icons/loading.png  
/opt/activitydiary/icons/running.png  
/opt/activitydiary/icons/start.gif  
/opt/activitydiary   
  
   
   
%changelog  
  
   
