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
	
	val startTime = System.currentTimeMillis();
	var lastFrameTime = startTime;
	while(true)
	{
		val currentTime = System.currentTimeMillis();
		val elapsedTime = currentTime - startTime;
		val deltaTime = currentTime - lastFrameTime;
		
		opmodeLoader.update(deltaTime / 1000.0, elapsedTime / 1000.0);
		
		lastFrameTime = currentTime;
	}
}