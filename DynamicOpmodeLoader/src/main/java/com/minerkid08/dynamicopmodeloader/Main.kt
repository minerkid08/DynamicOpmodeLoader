package com.minerkid08.dynamicopmodeloader

class TestObj
{
	fun dothing()
	{
		println("did thing");
	}
}

fun main()
{
	val opmodeLoader = OpmodeLoader();
	val opmodes = opmodeLoader.init() ?: return;
	
	val builder = opmodeLoader.getFunctionBuilder();
	val obj = TestObj();
	
	builder.setCurrentObject(obj);
	builder.addFun("dothing");
	
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