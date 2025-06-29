package com.minerkid08.dynamicopmodeloader

import java.io.File
import java.io.FileOutputStream
import java.net.ServerSocket

class FileServer
{
	companion object
	{
		fun start()
		{
			System.loadLibrary("dynamicopmodeloader");
			FileServer().startUnzipServer();
		}
	}

	fun startUnzipServer()
	{
		val stdlib = LuaStdlib();
		val thread = Thread({
			val serverSocket = ServerSocket(6969);
			while(true)
			{
				val socket = serverSocket.accept();
				val stream = socket.getInputStream();
				var bytes = 512;
				val file = File("/sdcard/data.pak");
				val outputStream = FileOutputStream(file);
				val arr = ByteArray(512);
				while(bytes == 512)
				{
					bytes = stream.read(arr, 0, 512);
					outputStream.write(arr, 0, bytes);
				}
				outputStream.close();
				unzip(stdlib, "/sdcard");
				socket.close();
			}
		});
		thread.start();
	}

	private external fun unzip(stdlib: LuaStdlib, path: String);
}