FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
  && apt-get install --no-install-recommends -y\
    ssh \
    gdb \
    git \
    make \
    clang \
    cmake \
    g++-10 \
    rsync \
    python3-pip \
  && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 100 \
  && update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-10 100 \
  && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 \
  && update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-10 100 \
  && apt-get clean autoclean \
  && apt-get autoremove --yes \
  && rm -rf /var/lib/{apt,dpkg,cache,log}

RUN ( \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_bsc \
  && mkdir /run/sshd \
  && useradd -m bsc \
  && yes bsc | passwd bsc
RUN apt-get install -y --no-install-recommends libmagic-dev

RUN adduser bsc sudo && printf "bsc ALL= NOPASSWD: ALL\\n" >> /etc/sudoers
RUN mkdir -p /home/bsc/.conan \
    && chsh -s /bin/bash bsc \
    && printf 'export PATH="${PATH}:/home/bsc/.local/bin"' >> ~/.bashrc \
    && printf 'eval "$(pyenv init -)"\n' >> ~/.bashrc \
    && printf 'eval "$(pyenv virtualenv-init -)"\n' >> ~/.bashrc \
    && chown -cR bsc:bsc /home/bsc \
    && pip3 install --system conan
USER bsc
WORKDIR /home/bsc
RUN conan config init
RUN conan profile update settings.compiler.libcxx=libstdc++11 default
RUN pip3 install virtualenv
COPY conan/conanfile.txt .
RUN conan install . -s build_type=Debug -s compiler=gcc -s compiler.version=10 -s compiler.libcxx=libstdc++11 -g=cmake --build missing
RUN conan install . -s build_type=Release -s compiler=gcc -s compiler.version=10 -s compiler.libcxx=libstdc++11 -g=cmake --build missing
USER root
CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_bsc"]