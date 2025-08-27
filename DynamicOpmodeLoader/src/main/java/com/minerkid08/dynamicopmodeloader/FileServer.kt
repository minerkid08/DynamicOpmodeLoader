package com.minerkid08.dynamicopmodeloader

import java.io.File
import java.io.FileOutputStream
import java.net.ServerSocket
import java.net.SocketAddress
import kotlin.math.min

object FileServer
{
	@JvmStatic
	fun start()
	{
		OpmodeLoader.loadLibrary();

		LuaStdlib.log("fserver", "server starting");
		val thread = Thread({
			val serverSocket = ServerSocket(6969);
			LuaStdlib.log("fserver", "server started");
			while (true)
			{
				val path = "/sdcard";
				val socket = serverSocket.accept();
				val stream = socket.getInputStream();

				val lenBytes = ByteArray(8);
				stream.read(lenBytes, 0, 8);
				val len: ULong =
					lenBytes.foldIndexed(0uL, { index, acc, byte -> acc + (byte.toULong() shl index * 8) });

				var bytesRead = 0uL;
				val file = File("$path/data.pak");
				val outputStream = FileOutputStream(file);
				val arr = ByteArray(512);
				while (bytesRead < len)
				{
					val bytes = stream.read(arr, 0, min(512, (len - bytesRead).toInt()));
					outputStream.write(arr, 0, bytes);
					bytesRead += bytes.toULong();
				}
				outputStream.close();
				unzip(path);
				socket.close();
			}
		});
		thread.start();
	}

	private external fun unzip(path: String);
}