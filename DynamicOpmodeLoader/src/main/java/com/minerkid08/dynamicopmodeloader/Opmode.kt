package com.minerkid08.dynamicopmodeloader

class Opmode(val name: String, val type: Int, val group: String)
{
	companion object
	{
		val Telop = 0;
		val Auto = 1;
	}
}