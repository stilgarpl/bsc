FROM conanio/gcc10

RUN sudo apt-get -qq update  && sudo apt-get -qq install -y --no-install-recommends ssh rsync gdb
RUN sudo apt-get -qq install -y --no-install-recommends libmagic-dev
COPY conan/conanfile.txt .
RUN conan install . --build missing && source activate.sh
CMD sudo service ssh start && tail -F /var/log/messages
