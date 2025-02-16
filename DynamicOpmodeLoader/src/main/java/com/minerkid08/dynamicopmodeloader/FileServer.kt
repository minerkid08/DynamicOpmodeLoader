package com.minerkid08.dynamicopmodeloader

import org.apache.ftpserver.FtpServer
import org.apache.ftpserver.FtpServerFactory
import org.apache.ftpserver.ftplet.User
import org.apache.ftpserver.listener.ListenerFactory
import org.apache.ftpserver.usermanager.PropertiesUserManagerFactory
import org.apache.ftpserver.usermanager.UserFactory
import org.apache.ftpserver.usermanager.impl.WritePermission

class FileServer
{
	companion object
	{
		fun start(user: User? = null)
		{
			val serverFactory = FtpServerFactory();
			val listenerFactory = ListenerFactory();
			listenerFactory.port = 2121;
			serverFactory.addListener("default", listenerFactory.createListener());
			val userManagerFactory = PropertiesUserManagerFactory();
			val userManager = userManagerFactory.createUserManager();
			val user2 = if(user == null)
			{
				val user3 = UserFactory();
				user3.name = "user";
				user3.password = "user";
				user3.homeDirectory = "/sdcard/lua";
				user3.authorities = listOf(WritePermission());
				user3.createUser();
			}
			else user;
			userManager.save(user2);
			serverFactory.userManager = userManager;
			val ftpServer: FtpServer = serverFactory.createServer();
			
			val lib = LuaStdlib();
			
			lib.print("FTP server starting");
			ftpServer.start();
			lib.print("FTP server started on port 2121");
		}
	}
}