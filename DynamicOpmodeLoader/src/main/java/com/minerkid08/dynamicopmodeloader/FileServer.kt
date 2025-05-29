package com.minerkid08.dynamicopmodeloader

import org.apache.ftpserver.FtpServer
import org.apache.ftpserver.FtpServerFactory
import org.apache.ftpserver.ftplet.User
import org.apache.ftpserver.listener.ListenerFactory
import org.apache.ftpserver.usermanager.PropertiesUserManagerFactory
import org.apache.ftpserver.usermanager.UserFactory
import org.apache.ftpserver.usermanager.impl.WritePermission
import java.net.ServerSocket

class FileServer
{
	companion object
	{
		private var mainUser: User? = null;
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
				val userBuilder = UserFactory();
				userBuilder.name = "user";
				userBuilder.password = "user";
				userBuilder.homeDirectory = "/sdcard/lua";
				userBuilder.authorities = listOf(WritePermission());
				userBuilder.createUser();
			}
			else user;
			mainUser = user;
			userManager.save(user2);
			serverFactory.userManager = userManager;
			val ftpServer: FtpServer = serverFactory.createServer();

			val lib = LuaStdlib();

			lib.print("FTP server starting");
			ftpServer.start();
			lib.print("FTP server started on port 2121");
			FileServer().startUnzipServer();
		}
	}

	fun startUnzipServer()
	{
		val thread = Thread();
		thread.run {
			val serverSocket = ServerSocket(6969);
			while(true)
			{
				val socket = serverSocket.accept();
				mainUser?.let { unzip(it.homeDirectory) };
				socket.close();
			}
		}
	}

	private external fun unzip(path: String);
}