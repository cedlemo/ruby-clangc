# -*- mode: ruby -*-
# vi: set ft=ruby :
Vagrant.configure(2) do |config|
  config.vm.box = "archlinux-x86_64"

  config.vm.provider "virtualbox" do |vb|
    # Display the VirtualBox GUI when booting the machine
    vb.gui = true
    # Customize the amount of memory on the VM:
    vb.memory = "1024"
  end
  shell_script = <<-SHELL
    yaourt -Syu --noconfirm
    yaourt --noconfirm -S clang llvm llvm-libs git cppunit ruby
    git clone https://github.com/cedlemo/ruby-clangc.git
    cd ruby-clangc
    gem build clangc.gemspec
    gem install clangc-0.0.1.gem
  SHELL
  config.vm.provision "shell", inline: shell_script, privileged: false
end
