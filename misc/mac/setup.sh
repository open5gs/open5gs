echo "Installing MongoDB on your Mac using Homebrew"
brew tap mongodb/brew
brew install mongodb-community@4.2

echo "Registering MongoDB as a startup service"
brew services start mongodb/brew/mongodb-community

declare -a dependencies=("iproute2mac"
                          "libmongoc"
                          "libidn"
                          "ninja"
                          "gnu-sed"
                          "libusrsctp"
                          "gnutls"
                          "libidn"
                          "libyaml"
                          "pkg-config"
                          "libgcrypt")

for dependency in ${dependencies[*]}
do
  if brew ls --version $dependency > /dev/null; then
    echo "[OK] $dependency is already installed."
  else
    echo "[WARN] $dependency is not installed. Going ahead and installing $dependency"
    brew install $dependency
  fi;
done


echo "Installing tools for IP"
brew install iproute2mac

brew install libmongoc
brew install libidn
brew install ninja

echo "Installing tuntap. This would need sudo permissions and an explicit allow for Kernel Extensions to load on mac"
brew cask install tuntap
echo "Follow the TN2459 Documentation to allow user approved kernel extension loading for tuntap. \\
      https://developer.apple.com/library/archive/technotes/tn2459/_index.html"

echo "Run the misc/netconf.sh to configure the required TUN devices and enable IP Forwarding"
echo "All dependencies are upto date. Ready to build with meson and ninja"
