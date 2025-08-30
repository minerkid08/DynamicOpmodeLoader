package com.minerkid08.dynamicopmodeloader

import java.io.File
import java.io.FileOutputStream
import java.io.InputStream
import java.io.InputStreamReader
import java.net.ServerSocket
import java.net.SocketAddress
import java.nio.ByteBuffer
import java.nio.ByteOrder
import kotlin.math.min

object FileServer
{
	var path = "/sdcard";

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
				val socket = serverSocket.accept();
				val stream = socket.getInputStream();

				val len = readLong(stream);

				var bytesRead = 0uL;
				File(path).mkdirs();
				val file = File("$path/data.pak");
				val outputStream = FileOutputStream(file);
				val arr = ByteArray(512);
				LuaStdlib.log("fserver", "receiving file of length %x".format(len.toLong()));
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

	fun readLong(inputStream: InputStream): ULong
	{
		val buffer = ByteArray(ULong.SIZE_BYTES);
		var bytesRead = 0;
		while (bytesRead < ULong.SIZE_BYTES)
		{
			val result = inputStream.read(buffer, bytesRead, ULong.SIZE_BYTES - bytesRead);
			if (result == -1)
			{
				error("failed to read byte");
			}
			bytesRead += result;
		}

		val byteBuffer = ByteBuffer.wrap(buffer);
		byteBuffer.order(ByteOrder.LITTLE_ENDIAN);

		return byteBuffer.long.toULong();
	}

	private external fun unzip(path: String);
}