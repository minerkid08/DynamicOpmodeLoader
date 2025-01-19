package com.minerkid08.dynamicopmodeloader

open class LuaType(val clazz: Class<*>)
{
	class Double : LuaType(kotlin.Double::class.java);
	class Bool : LuaType(Boolean::class.java);
	class String : LuaType(kotlin.String::class.java);
	class Void : LuaType(Any::class.java);
	class CheckRun : LuaType(Any::class.java);
	class Object(clazz: Class<*>) : LuaType(clazz);
}

class FunctionBuilder
{
	/**
	 * sets the object to pull functions from to expose to lua
	 */
	external fun <T> setCurrentObject(thing: T);
	
	external fun createClass(name: String);
	
	/**
	 * adds a function from java to lua
	 *
	 * if a class is active when this is called the function will be put in that table
	 * else the function will be set as a global
	 */
	fun objectAddFun(name: String, rtnType: LuaType = LuaType.Void(), argTypes: List<LuaType>? = null)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size ?: 0;
		addFunction(name, sig, typeToInt(rtnType), argc);
	}
	
	fun classAddFun(
		clazz: Class<*>, name: String, rtnType: LuaType = LuaType.Void(), argTypes: List<LuaType>? = null
	)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size ?: 0;
		addFunctionc(clazz, name, sig, typeToInt(rtnType), argc);
	}
	
	private external fun addFunction(name: String, funSignature: String, rtnType: Int, argc: Int);
	
	private external fun addFunctionc(
		clazz: Class<*>, name: String, funSignature: String, rtnType: Int, argc: Int
	);
	
	private fun generateSignature(rtnType: LuaType, argTypes: List<LuaType>?): String
	{
		return generateSignature(typeToStr(rtnType), argTypes);
	}
	
	private fun generateSignature(rtnType: String, argTypes: List<LuaType>?): String
	{
		var funSignature = "(";
		if(argTypes != null)
		{
			for(type in argTypes)
			{
				if(type is LuaType.CheckRun) throw LuaError("CheckRun is not a valid argument type");
				if(type is LuaType.Void) throw LuaError("Void is not a valid argument type");
				funSignature += typeToStr(type);
			}
		}
		funSignature += ')';
		funSignature += rtnType;
		
		funSignature = funSignature.replace('.', '/');
		
		return funSignature;
	}
	
	private fun typeToStr(type: LuaType): String
	{
		if(type is LuaType.Double) return "D";
		if(type is LuaType.Bool) return "Z";
		if(type is LuaType.Void) return "V";
		if(type is LuaType.CheckRun) return "Z";
		if(type is LuaType.String) return "Ljava/lang/String;";
		return 'L' + type.clazz.name.replace('.', '/') + ';';
	}
	
	private fun typeToInt(type: LuaType): Int
	{
		if(type is LuaType.Double) return 3;
		if(type is LuaType.Bool) return 1;
		if(type is LuaType.Void) return 0;
		if(type is LuaType.CheckRun) return -1;
		if(type is LuaType.String) return 4;
		if(type is LuaType.Object) return 5;
		return 0;
	}
}