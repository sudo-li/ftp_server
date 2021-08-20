ftp_server
==========

A simple TCP based FTP server,which supports **list**,**get**,**put** and **cd**.Subsequent functions are still under development.

## Building

**Please modify the IP address and compile it before use. The location of the moditied IP is in the datatype.h file under the client/include directory.**  

You must have cmake and make and gcc install.Execute the following three commands to complete the compilation,and the executable fiel will be generated in the bin directory under the project directory.  
```
cd build  
cmake ../
make
```

## Usage  

### server

Fro the server,you only need to listen to a certain port.  
For example  
```
./server 65000
```

### client

For the client,you need to specify an IP and the corresponding port.  
For example  
```
./client 127.0.0.1 65000
```

### user
The server has the function of user authentication.Before use,you need to add you username and password to the .auth file int the server directory.The format is`username password`

### command
```
list
get Server file name
put Local file name
```
