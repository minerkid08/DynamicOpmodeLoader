package com.minerkid08.dynamicopmodeloader

class Opmode
{
	lateinit var name: String;
	var type: Int = 0;
	var group: String? = null;
	var order: String? = null;

	companion object
	{
		const val Telop = 0;
		const val Auto = 1;
	}
}