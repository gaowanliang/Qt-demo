# Qt-demo
This demo implements a C language registration / login server and a QT client

##	Register the service side
First, we learned about the configuration of vim under Linux, which does not fit the usage habits because vim was originally configured without a line number and with an automatic indentation of 8 spaces. We need to be in home. . The relevant parameters, including tabstop,smartindent, etc., are configured in the vimrc file and are notcovered here.  
The goal we're going to achieve in the first place is to implement a registered feature, so on the service side we need to do some of the following:
  1.	Implement communication with Qt clients. 
  2.	Get the username and password (encrypted) from the Qtclient. 
  3.	Create the database and put the registered contents into the database to determine if the user name is correct by the information returned by the database.
  4.	Returns information about the success/failure of the registration.


###	Initialize the Socket

In order to complete the a function, we need to use Socket programming in C language, we first need to use sys/socket The relevant procedure in h. 

In C language, there are two types of supported Sockset, one is a streaming socket, the other is a data series socket, because we know that streaming sockets, also known as TCP, need to be used three times during the handshake four waves. It is more troublesome to deal with it in C language. So we decided to use a dataser socket, or UDP, in our project. Verification  is not required, so it is relatively simple to handle. 

To program with Socket, the first step is to initialize The socket,  bind IP, and port, and enter the listening state. We're using the IPv4 address and port 7788, which are bound and then sent to the listening state through a dead loop. 
###	Initialize the database

Then we need to create a database, in the face of many databases, we choose the embedded platform is relatively friendly to SQLITE3. SQLite is an in-process library that implements a self-sufficient, serverless, zero-configuration, transactional SQL database engine. It is a zero-configuration database, which means that unlike other databases, you do not need to configure it in the system. Like other databases, the SQLite engine is not a stand-alone process that can be connected statically or dynamically to the needs of the application. SQLite has direct access to its stored files. A system (serverless) that does not require a separate server process or operation. SQLite does not require configuration, which means no installation or management is required. A complete SQLite database is stored in a single cross-platform disk file. SQLite is very small, lightweight, less than 400KiB when fully configured, and less than 250KiB when optional feature configurations are omitted. SQLite is self-sufficient, which means no external dependency is required. SQLite transactions are fully ACID-compatible and allow safe access from multiple processes or threads. SQLite supports the functionality of most query languages of the SQL92 (SQL2) standard. SQLite is written using ANSI-C and provides a simple and easy-to-use API. SQLite runs in UNIX (Linux, Mac OS-X, Android, iOS) and Windows (Win32, WinCE, WinRT).  So we chose SQLite3

Operating the SQLite3 database using C in Linux system requires the installation of Libsqlite3-dev, which is also introduced in C language. h Library. We use the relevant commands to create a database, that is, sqlite3_open,which successfully returns SQLITE_OK, to determine whether the database was opened successfully. At the same time, using the sqlite3_exec function to execute the sql statement, since we only need to implement a registration function, we only need two headers directly, that is, the user name and password, and finally close the database through the sqlite3_close, that is, the initialization of the database. 
###	Convention communication rules

The data type convention between the service side and the client is required in order to communicate reasonably, so in order to realize the communication between the two ends, we must carry out the contract of the communication rules, we can intuitively feel that there are several quantities that need to be agreed upon:
1.	The IP address of the communication
2.	The port of communication
3.	The user name
4.	password
5.	The maximum length of the user name at the time of communication
6.	The maximum length of the password at the time of communication
7.	Registration status
Where registration status is a priority, for the registration scenario, we can find out from our daily experience that the following states must be included:
1.	Registration was successful
2.	The registered user name duplicates the user name in the database
3.	The password at registration does not comply with the rules
The above states are both ends of the need to be agreed upon, otherwise in the actual communication can not be interpreted on the returned information, the actual implementation, you can refer to the appendix proto.h section, here will not be repeated. 
###	The listening process
After describing the initialization process above, the next step is to listen to the client, first we need to use recvfrom to receive the data returned by the client. Once we receive the information from the database, we need fork a child process, which is done to ensure that the parent process can also maintain the ability to get information in the event of a problem with the child process, and that this action also improves the concurrency of the server. 

In the sub-process, we need to make a judgment, based on the three states presented in 2.1.3.  The real process of judgment is that first we need to connect to the database, create a database cursor, and then  traverse the user name of the SQLITE3 database through SQL statements.  If the cursor traverses the entire database, triggers SQLITE_DONE and then does not find the corresponding user name, we can then determine that the user name can be used, so we can set the registration status in the data type we specify as successful registration, and use sqlite3_exec to execute the SQL statement inserting our username and password, i.e. the user's registration is complete. When our cursor stops in the middle of the database, that is, in the SQLTIE_ROW state, that means the same user name is found in the database, so we will judge at this point that the user name has been used, so at this point we can return the returned data as the user name already exists. 

Once we've made our judgment, we can return data to the client through the sendto function, and our service side is basically set up. 
 

##	Register the client
What you need to register a client is an input box for entering your account number and password, and a submit button, some of which need to be done as follows. 
###	Convention communication rules
Data type conventions are required between the server and the client in order to communicate reasonably, and since we have described the server's communication rules in 2.1.3, we also need to configure the corresponding communication rules on the client side, just copy the proto.h header file, not to be repeated here. 
###	Configure the interface
First of all, we refer to the common login interface, you can clearly know that the registration interface is a three-part, that is, the registration username input box, registration password input box, submit the registration button. This is a classic vertical three-component layout, so we just need to declare one QVBOXLAYOUT and then declare two LineEdit and one Pushton.  At the same time, the general components are not placed in the top grid, so here we need to set layout margin, according to the specific situationto adjust, here will not be repeated, please refer to rgswidget cpp。 
###	Send data
When sending data, we need to use Qt's  Socket communication library, and since we've already said that we communicate through a dataser socket, or UDP, we need to introduce a QUdpSocket library here to implement a socket like UDP.  

First we need to put a slot function on the send button, which is to trigger the send operation when the button is pressed. A QUdpSocket object  is also required as the basis for the next communication. 

You then need to get the information from the two input boxes, and for the information we get about the user name, we need to copy the information into the username field in our custom data structure through the string copy function strcpy.  But in Qt we use the QString type, and in our custom data type, we use the array type of char, and the two data types are not the same, so we need to convert the data type to prevent errors. The method of conversion is also simple: the data is converted directly to the byte array type through toLatin1, so that communication does not result in errors due to different data types. 

And for passwords, which are more sensitive data, we can't send it in clear text, so we need to encrypt the data relatively simply, and the method is simple, we call the QCryptHash library to encrypt the data,   and here we use the more secure Sha512 algorithm. We then convert the encrypted binary data to a 16-in string, and then copy the data into our custom data structure. 

Eventually, we can get the data returned by the server by sending the custom data structure specified in our protocol to the server via the port and IP address specified in the protocol through the writeDatagram method. 
###	Receive and resolve
The process of receiving the server side is also relatively complex. First we need to define a slot function, that is, in Qt's  Socket class, there is a slot function called readyRead, which implements a slot function that is executed when the Socket signal is passed in, so we need to put this slot function on our Socket object. 

When we receive a packet, we need readDatagram to parse the packet, and then we can take the registration information out of our custom data structure. As we agreed, if the registration is successful, we can prompt the registration success through the prompt box, and return to the login interface, if the server returns a user name already exists, we can ask the user through the prompt box whether the user needs to re-enter, if necessary, we need to re-empty the input box. If not, we will close the window directly. 

At this point, the full points of registering the client are complete.

 

##	Sign in to the service side
Since the login service side is almost identical to most of the registered service side, it is not repeated here to explain the differences directly. 
###	Convention communication rules
The contract between the service side and the client needs to be data type in order to communicate reasonably, so in order to realize the communication between the two ends, we must carry out the contract of the communication rules, we can intuitively feel that there are several quantities that need to be agreed upon:
1.	The IP address of the communication
2.	The port of communication
3.	The user name
4.	password
5.	The maximum length of the user name at the time of communication
6.	The maximum length of the password at the time of communication
7.	Sign-in status

Where the port cannot use the same port as the registration, there will be a conflict. At the same time login status is to be considered, for the registration of this scenario, we can through the daily experience, we can find the following states must be included:
1.	The login was successful
2.	The user name does not exist
3.	The password is wrong

The above states are both ends of the need to be agreed upon, otherwise in the actual communication can not be interpreted on the returned information, the actual implementation, you can refer to the loginProto.h section of the appendix, here will not be repeated. 

###	The listening process
After describing the initialization process above, the next step is to listen to the client, first we need to use recvfrom to receive the data returned by the client. Once we receive the information from the database, we need fork a child process, which is done to ensure that the parent process also maintains the ability to get information in the event of a problem with the child process, and that this action also improves the concurrency of the server. 

In the sub-process, we need to make a judgment, based on the three states presented in 2.3.1.  The real-world judgment process is this: first we need to connect to the database, create a database cursor, and then  traverse the user name of the SQLITE3 database through SQL statements.  If the cursor traverses the entire database, triggers SQLITE_DONE and then does not find the corresponding user name, we can then determine that the user name does not exist, so we can set the login status in the data type we specify to be that the user name does not exist. When our cursor stops in the middle of the database, that is, in the SQLTIE_ROW state, that means the same user name is found in the database, so we'll judge that the username exists at this point, so we need to determine if the password is correct. We can determine the row where the current behavior username is located through the cursor, so the second item of this line, the password we need to compare, we just need to use the sqlite3_column_text this command can specify the out of the column, if the removal of this item compared to the password sent is correct, you can judge the success of the login, if the comparison found incorrectly, the password error. 

Once we've made our judgment, we can return data to the client through the sendto function, and our service side is basically set up. 
 

##	Sign in to the client
Since the login customer side is almost identical to most of the content on the registered customer side, it is not repeated here and explains the differences directly. 
###	Configure the interface
First of all, we refer to the common login interface, you can clearly know that the login interface is a four-part, that is, login username input box, login password input box, registration button, submit registration button. This is a classic vertical three-component layout, so we just need to declare one QVBOXLAYOUT and one QHBOXLAYOUT, and then declare two LineEdit and two PushButtons.  Then put two PushButtons in QHBOXLAYOUT and insert them into QVBOXLAYOUT.  At the same time, the general components are not placed in the top grid, so here we need to set layout  margin,according to the specific situation to adjust, here will not be repeated, please refer to loginwidget cpp。 
###	Receive and resolve
The process of receiving the server side is also relatively complex. First we need to define a slot function, that is, in Qt's  Socket class, there is a slot function called readyRead, which implements a slot function that is executed when the Socket signal is passed in, so we need to put this slot function on our Socket object. 

When we receive a packet, we need readDatagram to parse the packet, and then we can take the registration information out of our custom data structure. As we agreed, if the login is successful, we can prompt the registration success through the prompt box, and jump to the main interface, if the server returns a user name does not exist or password error, we can ask the user through the prompt box whether the user needs to re-enter, if necessary, we need to re-empty the input box. If not, we will close the window directly. 

At this point, the full points of the login client are complete.
 

##	The main interface of the client
Since the client main interface technical points are not many, here are some of the more important points. 
###	Picture button
The main interface is made up of 10 graphics buttons, and for one button, we need to implement the graphics buttons as follows
1.	Set Text to empty
2.	Set Icon to the picture you want
3.	Adjust the picture to the appropriate size
4.	Set the button to flat
5.	Cancel the border of the button
 

###	Back button
Returning to the previous window requires us to define a signal and a slot ourselves, first we need to bind a slot to the new interface to be generated in the interface to be returned, which we define ourselves, and if the slot receives a signal, it will show the original hidden window. 

In the new window, we just need to create a new signal, when clicking the back button, directly destroy the current window, and send the signal, to achieve the effect of returning to the next layer.
##	The Living Room window interface
This interface has the following technical points
###	Displays the current system time
We can get the current time of the system using currentTime in the QTime class, and then we need to convert the time to a string in hh:mm:ss format, and since we need to display it on the LCD lcd assembly, we need to use a timer, bind the timer to a slot function, and then let the timer execute once in 1000 milliseconds. 

To simulate the actual display, we need to make two colons flash, where we can create a bool type variable that reverses every time a timer event is executed. A colon is displayed when it is true, and a colon is not displayed when it is false, which completes the effect. 
 

###	Video playback
Video playback we only need to use the QMediaPlayer library and QVideoWidget to do this, we just need  a new QMediaPlayer object and a QVideoWidget object, the QVideoWidget object to the UI component through addWidget, and then we use setMedia to set the file.  SetVideoOutput settings play. 
 
