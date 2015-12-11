#secnet</br>   
-----------------lhash----------------     
openssl库中hash table的实现和使用demo</br>     

----------------stack-----------------      
openssl库中stack的实现和使用demo</br>    

=============================================
echo_server           
=============================================

=============================================
Project Introduction   
=============================================
1.Build A Echo Server which is based in openssl library.     
2.The Server create nonblock socket and use epoll ET mode to communicate with client.    

=============================================
Build and use
=============================================
1. 'cd echo_server'
2. run 'make' to produce SSLServer and SSLClient.   
3. run 'SSLServer' to listen network event.   
4. run 'SSLClient message' to send request to server.   

Thanks for looking .       


