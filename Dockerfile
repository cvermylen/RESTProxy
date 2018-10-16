FROM alpine:3.7
RUN apk update 
RUN apk add cmake
RUN apk add make
RUN apk add clang
