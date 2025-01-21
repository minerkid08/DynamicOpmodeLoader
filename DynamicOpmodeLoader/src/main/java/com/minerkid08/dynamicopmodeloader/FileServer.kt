package com.minerkid08.dynamicopmodeloader

import android.os.Environment
import android.util.Log
import java.io.File
import java.net.ServerSocket

class FileServer {
	companion object
	{
		fun start()
		{
			val storageDir = Environment.getExternalStorageDirectory();
			val t = Thread {
				run {
					val server = ServerSocket(6969);
					Log.d("FServer", "server started");
					while(true)
					{
						val socket = server.accept();
						var str = "";
						var a2 = "lua";
						var arg = 1;
						val input = socket.getInputStream();
						var len: Int;
						val buf = ByteArray(1);
						var writing = false;
						do
						{
							len = input.read(buf);
							if(buf[0].toInt() == 2) break;
							if(buf[0].toInt().toChar() == '\t' && !writing) writing = true;
							else if(buf[0].toInt().toChar().code == 3)
							{
								arg = 2;
								a2 = "";
								continue;
							}
							else if(writing)
							{
								if(arg == 1) str += buf[0].toInt().toChar();
								else a2 += buf[0].toInt().toChar();
							}
						}
						while(len > 0);
						Log.d("FServer", a2);
						if(str == "Get")
						{
							Log.d("FServer", "get");
							val d = File("$storageDir/$a2");
							var out = "";
							d.walk().forEach { file ->
								if(file.isFile)
								{
									Log.d("FServer", "reading ${file.path}");
									out += file.relativeTo(storageDir).toString() + '\n';
									out += file.readBytes().toString(Charsets.UTF_8) + '\b';
								}
							}
							out += '\r';
							socket.getOutputStream().write(out.toByteArray());
						}
						else if(str == "Remove")
						{
							Log.d("FServer", "remove");
							if(File("$storageDir/$a2").path == storageDir.toString()) socket.getOutputStream()
								.write("you are not allowed to nuke the external storage directory on the robot\r".toByteArray());
							else
							{
								File("$storageDir/$a2").walk()
									.forEach { file -> if(file.isFile) file.delete(); else file.deleteRecursively(); }
								Log.d("FServer", "done");
								socket.getOutputStream().write("done\r".toByteArray());
							}
						}
						else if(str == "List")
						{
							Log.d("FServer", "list");
							val d = File("$storageDir/$a2");
							var out = "";
							var count = 0;
							var size: Long = 0;
							d.walk().forEach { file ->
								if(file.isFile)
								{
									out += file.relativeTo(storageDir).toString() + '\n';
									size += file.length();
									count++;
								}
							}
							out += count;
							out += " files\n";
							out += size;
							out += " bytes\n\r";
							socket.getOutputStream().write(out.toByteArray());
						}
						else
						{
							Log.d("FServer", "write");
							var path = "";
							var data = "";
							var d = false;
							for(s in str)
							{
								if(s == '?' && !d) d = true;
								else if(s.code == 1)
								{
									val a = path.split('/');
									var folders = "";
									for(i in 0..a.size - 2)
									{
										folders += a[i];
										if(i < a.size - 2)
										{
											folders += '/';
										}
									}
									File("$storageDir/$folders").mkdirs();
									val file = File(storageDir, path);
									if(!file.exists())
									{
										file.createNewFile();
									}
									val c = file.outputStream();
									c.write(data.toByteArray());
									c.close();
									Log.d("FServer", "wrote $path");
									path = "";
									data = "";
									d = false;
								}
								else if(!d) path += s;
								else data += s;
							}
							socket.getOutputStream().write("done\r".toByteArray());
						}
						socket.close();
					}
				}
			}
			t.start()
		}
	}
}