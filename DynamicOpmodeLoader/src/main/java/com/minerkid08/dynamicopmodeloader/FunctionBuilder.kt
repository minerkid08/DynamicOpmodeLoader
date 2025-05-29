package com.minerkid08.dynamicopmodeloader

abstract class LuaType(val clazz: Class<*>)
{
	companion object
	{
		val Void = VoidT();
		val Bool = BoolT();
		val Double = DoubleT();
		val String = StringT();
		val Builder = BuilderT();
	}

	class Object(clazz: Class<*>): LuaType(clazz);
	class DoubleT: LuaType(kotlin.Double::class.java);
	class BoolT: LuaType(Boolean::class.java);
	class StringT: LuaType(kotlin.String::class.java);
	class VoidT: LuaType(Any::class.java);
	class BuilderT: LuaType(Any::class.java);
}

class FunctionBuilder
{
	/**
	 * sets the object to pull object functions from
	 */
	external fun <T> setCurrentObject(thing: T);

	/**
	 * creates a class for a lua function to return
	 * does not need to be called if a function is created for that class
	 */
	external fun createClass(name: String);

	/**
	 * adds a function from a java object to lua as a global
	 */
	fun addObjectFunction(
		name: String,
		rtnType: LuaType = LuaType.Void,
		argTypes: List<LuaType>? = null
	)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size?:0;
		addFunction(name, sig, typeToInt(rtnType), argc);
	}

	/**
	 * adds a function from a java class to a class table in lua
	 */
	fun addClassFunction(
		clazz: Class<*>, name: String, rtnType: LuaType = LuaType.Void, argTypes: List<LuaType>? = null
	)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size?:0;
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
				if(type is LuaType.VoidT) throw LuaError("Void is not a valid argument type");
				if(type is LuaType.BuilderT) throw LuaError("Builder is not a valid argument type");
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
		return when(type)
		{
			is LuaType.DoubleT -> "D";
			is LuaType.BoolT -> "Z";
			is LuaType.VoidT -> "V";
			is LuaType.BuilderT -> "V";
			is LuaType.StringT -> "Ljava/lang/String;";
			else -> "L${type.clazz.name.replace('.', '/')};";
		}
	}

	private fun typeToInt(type: LuaType): Int
	{
		return when(type)
		{
			is LuaType.DoubleT -> 3;
			is LuaType.BoolT -> 1;
			is LuaType.VoidT -> 0;
			is LuaType.StringT -> 4;
			is LuaType.Object -> 5;
			is LuaType.BuilderT -> 0xff;
			else -> 0;
		}
	}
}