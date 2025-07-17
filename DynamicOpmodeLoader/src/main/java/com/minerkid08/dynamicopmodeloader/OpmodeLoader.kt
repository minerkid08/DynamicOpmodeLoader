package com.minerkid08.dynamicopmodeloader

class OpmodeLoader
{
	companion object
	{
		private var loaded = false;
		@JvmStatic
		fun loadLibrary()
		{
			if(!loaded)
			{
				loaded = true;
				System.loadLibrary("dynamicopmodeloader");
			}
		}
	}
	private val stdlib = LuaStdlib();
	private val builder = FunctionBuilder();
	
	init
	{
		loadLibrary();

		internalInit2(stdlib);
		
		builder.setCurrentObject(stdlib);
		
		builder.addObjectFunction("print", LuaType.Void, listOf(LuaType.String));
	}
	
	/**
	 * Returns a function builder object for exposing functions to lua.
	 */
	fun getFunctionBuilder(): FunctionBuilder
	{
		return builder;
	}
	
	/**
	 * Initializes the lua instance and returns a list with all of the opmode names.
	 */
	fun init(): Array<String>?
	{
		return internalInit();
	}

	/**
	 * Closes the lua instance and cleans up all references to jobjects.
	 */
	external fun close();

	/**
	 * Loads an opmode to be run and calls init on that opmode.
	 */
	external fun loadOpmode(name: String);
	
	/**
	 * Calls start on the opmode with recognition as the first argument.
	 */
	external fun start(recognition: Int = 0);
	
	/**
	 * Calls the update function on the lua opmode with deltaTime as the first argument and elapsedTime as the second.
	 */
	external fun update(deltaTime: Double, elapsedTime: Double);
	
	/**
	 * Calls a global function with args as the arguments.
	 */
	external fun callFun(name: String, vararg args: Any);
	
	/**
	 * Calls a function in the opmode table with args as the arguments.
	 */
	external fun callOpmodeFun(name: String, vararg args: Any);
	
	private external fun internalInit2(luaStdlib: LuaStdlib);
	private external fun internalInit(): Array<String>?;
}