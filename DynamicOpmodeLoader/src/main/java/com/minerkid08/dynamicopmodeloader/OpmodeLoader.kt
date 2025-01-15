package com.minerkid08.dynamicopmodeloader

class OpmodeLoader()
{
	companion object
	{
		init
		{
			System.loadLibrary("dynamicOpmodeLoader");
		}
	}
	
	private val stdlib = LuaStdlib();
	private val builder = FunctionBuilder();
	
	/**
	 * returns a function builder object for exposing functions to lua
	 */
	fun getFunctionBuilder(): FunctionBuilder
	{
		return builder;
	}
	
	/**
	 * initalizes the lua instance and returns a list with all of the opmode names
	 */
	fun init(): Array<String>?
	{
		val opmodes = internalInit(stdlib);
		
		//builder.setCurrentObject(stdlib);
		
		builder.addFun("print", LuaType.Void, listOf(LuaType.String));
		builder.addFun("err", LuaType.Void, listOf(LuaType.String));
		builder.addFun("stop", LuaType.CheckRun);
		
		return opmodes;
	}
	
	/**
	 * loads an opmode to be run
	 */
	external fun loadOpmode(name: String);
	
	/**
	 * starts the opmode
	 */
	external fun start(recognition: Int = 0);
	
	/**
	 * calls the update function on the lua opmode with a Time object as the first argument
	 */
	external fun update(deltaTime: Double, elapsedTime: Double): Boolean;
	
	
	private external fun internalInit(luaStdlib: LuaStdlib): Array<String>?;
}