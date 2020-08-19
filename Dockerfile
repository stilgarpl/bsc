FROM conanio/gcc10

RUN sudo apt-get -qq update  && sudo apt-get -qq install -y --no-install-recommends ssh rsync gdb
RUN sudo apt-get -qq install -y --no-install-recommends libmagic-dev \
    && cd /tmp && git clone https://github.com/pantor/inja.git && cd inja \
    && cmake -Bbuild -H. \
    && sudo cmake --build build/ --target install
COPY conan/conanfile.txt .
RUN conan install . --build missing
CMD sudo service ssh start && tail -F /var/log/messages
