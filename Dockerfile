FROM alpine:3.7
RUN apk update 
RUN apk add ca-certificates
RUN update-ca-certificates
RUN apk add openssl
RUN apk add cmake
RUN apk add build-base
RUN apk add clang
RUN wget -O /tmp/criterion.tar.bz2 https://github.com/Snaipe/Criterion/releases/download/v2.3.2/criterion-v2.3.2-linux-x86_64.tar.bz2
RUN tar xvjf /tmp/criterion.tar.bz2
#RUN mv criterion-v2.3.2-linux-x86_64/include/* /usr/include/
#RUN mv criterion-v2.3.2-linux-x86_64/lib/* /usr/lib/
#RUN ls -l /usr/lib
