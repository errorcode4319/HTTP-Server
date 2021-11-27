# HTTP-Server   
c++ Based HTTP-Server

## Environment
 - Linux (amd64, arm)

## Dependencies
 - gcc (>=7, c++17)
 - make 
 - cmake (>=3.1)

## Usage
### Build
```sh
git clone https://github.com/errorcode4319/HTTP-Server.git
cd HTTP-Server
cd build
cmake..
make -j4 
./http-server
```

### Commands
```sh
> help
# Show commands description.
> run    
# start http service
> stop
# stop http service
> state
# show http service info
> restart
# restart http service 
> reload
# auto reload static html pages
# Not supported yet (2021-11-27)
> quit
# exit application 
```
