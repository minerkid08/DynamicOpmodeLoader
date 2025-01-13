package com.minerkid08.dynamicopmodeloader

fun main()
{
	System.loadLibrary("dynamicopmodeloader")
	
	val opmodeLoader = OpmodeLoader();
	val opmodes = opmodeLoader.init() ?: return;
	
	for(opmode in opmodes)
	{
		println("found opmode: $opmode");
	}
	
	opmodeLoader.loadOpmode(opmodes[0]);
	
	opmodeLoader.start();
}