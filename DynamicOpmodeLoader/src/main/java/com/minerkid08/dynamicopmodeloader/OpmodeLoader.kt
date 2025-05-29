package com.minerkid08.dynamicopmodeloader

class OpmodeLoader()
{
	companion object
	{
		init
		{
			System.loadLibrary("dynamicopmodeloader");
		}
	}
	
	private val stdlib = LuaStdlib();
	private val builder = FunctionBuilder();
	
	init
	{
		internalInit2(stdlib);
		
		builder.setCurrentObject(stdlib);
		
		builder.addObjectFunction("print", LuaType.Void, listOf(LuaType.String));
		builder.addObjectFunction("err", LuaType.Void, listOf(LuaType.String));
	}
	
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
		return internalInit();
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
	 * calls the update function on the lua opmode with deltaTime as the first argument and elapsedTime as the second
	 */
	external fun update(deltaTime: Double, elapsedTime: Double);
	
	/**
	 * calls a global function with args as the arguments
	 */
	external fun callFun(name: String, vararg args: Any);
	
	/**
	 * calls a function in the opmode table with args as the arguments
	 */
	external fun callOpmodeFun(name: String, vararg args: Any);
	
	private external fun internalInit2(luaStdlib: LuaStdlib);
	private external fun internalInit(): Array<String>?;
}