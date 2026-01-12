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
	private val builder = FunctionBuilder();
	
	init
	{
		loadLibrary();

		internalInit();
		builder.setCurrentClass(LuaStdlib::class.java);

		builder.addStaticFunction("print", LuaType.Void, listOf(LuaType.String));
		builder.pushTable("OpmodeType");
		builder.pushValuei("Telop", 0);
		builder.pushValuei("Auto", 1);
		builder.popTable();
	}

	/**
	 * Starts generating the definition file
	 * Once OpmodeLoader::init is called the state closes itself and errors to signal that it finished
	 */
	external fun genDefinitionFile();

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
	external fun init(): Array<Opmode>?

	/**
	 * Closes the lua instance and cleans up all jobject references.
	 */
	external fun close();

	/**
	 * Loads an opmode to be run and calls init on that opmode.
	 * @param name the name of the opmode to load.
	 */
	external fun loadOpmode(name: String);
	
	/**
	 * Calls start on the opmode with recognition as the first argument.
	 */
	external fun start(recognition: Int = 0);

	/**
	 * Calls stop on the opmode.
	 */
	external fun stop();

	/**
	 * Calls the update function on the lua opmode with deltaTime as the first argument and elapsedTime as the second.
	 * @param deltaTime time between last call and this one
	 * @param elapsedTime time that the opmode has been running
	 * @return should the opmode stop
	 */
	external fun update(deltaTime: Double = 0.0, elapsedTime: Double = 0.0): Boolean;
	
	/**
	 * Calls a global function with args as the arguments.
	 */
	external fun callFun(name: String, vararg args: Any);
	
	/**
	 * Calls a function in the opmode table with args as the arguments.
	 */
	external fun callOpmodeFun(name: String, vararg args: Any);
	
	private external fun internalInit();
}