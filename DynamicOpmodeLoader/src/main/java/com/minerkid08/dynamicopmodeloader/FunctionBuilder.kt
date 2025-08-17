package com.minerkid08.dynamicopmodeloader

import com.minerkid08.dynamicopmodeloader.error.FunctionBuilderError
import java.lang.reflect.Method

annotation class OpmodeLoaderBuilderFunction;
annotation class OpmodeLoaderFunction;

private enum class LuaTypeId(val id: kotlin.Int)
{
	None(-1),
	Nil(0),
	Bool(1),
	LightUserData(2),
	Number(3),
	String(4),
	Table(5),
	Function(6),
	UserData(7),
	Thread(8),
	Float(9),
	Int(10),
	Builder(11)
}

/**
 * Type for callbacks to lua.
 * Should not be created on its own, instead it is supplied as a function argument.
 */
class LuaCallback(private val id: Int)
{
	external fun call(vararg args: Any?);
}

/**
 * Used to expose functions from java classes or objects to lua.
 */
class FunctionBuilder
{
	/**
	 * Sets the object to pull object functions from.
	 */
	external fun <T> setCurrentObject(thing: T);

	/**
	 * Creates a class for a lua function to return.
	 * Does not need to be called if a function is created for that class.
	 */
	external fun createClass(name: String);

	/**
	 * Iterates through all methods on a class with the OpmodeLoaderFunction and OpmodeLoaderBuilderFunction annotations and adds them to a lua class;
	 */
	fun addClassAsClass(clazz: Class<*>)
	{
		val toAdd = ArrayList<Method>();
		val toAddBuilder = ArrayList<Method>();
		val methods = clazz.declaredMethods;
		for (method in methods)
		{
			var add = false;
			var addBuilder = false;
			for (annotation in method.annotations)
			{
				if (annotation.annotationClass == OpmodeLoaderFunction::class)
				{
					add = true;
					break;
				}
				if (annotation.annotationClass == OpmodeLoaderBuilderFunction::class)
				{
					addBuilder = true;
					break;
				}
			}
			if (add)
				toAdd.add(method)
			if (addBuilder)
				toAddBuilder.add(method)
		}
		if (toAdd.isEmpty() && toAddBuilder.isEmpty())
			return;

		for (method in toAdd)
		{
			val args = ArrayList<LuaType>();
			for (arg in method.parameterTypes)
			{
				println(arg.name);
				args.add(stringToLuaType(arg.name));
			}
			addClassFunction(clazz, method.name, stringToLuaType(method.returnType.name), args);
		}
		for (method in toAddBuilder)
		{
			val args = ArrayList<LuaType>();
			for (arg in method.parameterTypes)
			{
				args.add(stringToLuaType(arg.name));
			}
			addClassFunction(clazz, method.name, LuaType.Builder, args);
		}
	}

	/**
	 * Takes a class and adds all of its functions with the OpmodeLoaderFunction annotation to lua as global functions.
	 */
	fun addClassAsGlobal(clazz: Class<*>)
	{
		val constructor = clazz.constructors[0];
		val obj = constructor.newInstance();
		addObjectAsGlobal(obj)
	}

	/**
	 * Takes a object and adds all of its functions with the OpmodeLoaderFunction annotation to lua as global functions.
	 */
	fun <T> addObjectAsGlobal(obj: T)
	{
		if (obj == null)
			throw FunctionBuilderError("object cannot be null");

		val clazz = obj!!::class.java;
		val toAdd = ArrayList<Method>();
		val methods = clazz.methods;
		for (method in methods)
		{
			var add = false;
			for (annotation in method.annotations)
			{
				if (annotation.annotationClass == OpmodeLoaderFunction::class)
				{
					add = true;
					break;
				}
			}
			if (add)
				toAdd.add(method)
		}
		if (toAdd.isEmpty())
			return;

		setCurrentObject(obj);
		for (method in toAdd)
		{
			val args = ArrayList<LuaType>();
			for (arg in method.parameterTypes)
			{
				args.add(stringToLuaType(arg.name));
			}
			addObjectFunction(method.name, stringToLuaType(method.returnType.name), args);
		}
	}

	/**
	 * Adds a function from a java object to lua as a global.
	 */
	fun addObjectFunction(
		name: String, rtnType: LuaType = LuaType.Void, argTypes: List<LuaType>? = null
	)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size ?: 0;
		addFunction(name, sig, typeToInt(rtnType), argc);
	}

	/**
	 * Adds a function from a java class to a class table in lua.
	 */
	fun addClassFunction(
		clazz: Class<*>,
		name: String,
		rtnType: LuaType = LuaType.Void,
		argTypes: List<LuaType>? = null
	)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size ?: 0;
		addFunctionc(clazz, name, sig, typeToInt(rtnType), argc);
	}

	/**
	 * Pushes a table to the stack.
	 * When an object function is added, it will be placed into this table.
	 * If there is already a table on the stack then the new table will be placed inside of it.
	 *
	 * Tables have to be popped for you to be able to access it in lua.
	 */
	external fun pushTable(name: String);

	/**
	 * Pops a table off of the stack.
	 * A table has to be popped for you to be able to access it in lua.
	 */
	external fun popTable();

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
		if (argTypes != null)
		{
			for (type in argTypes)
			{
				if (type is LuaType.VoidT) throw FunctionBuilderError("Void is not a valid argument type");
				if (type is LuaType.BuilderT) throw FunctionBuilderError("Builder is not a valid argument type");
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
		return when (type)
		{
			is LuaType.DoubleT   -> "D";
			is LuaType.FloatT    -> "F";
			is LuaType.IntT      -> "I";
			is LuaType.BoolT     -> "Z";
			is LuaType.VoidT     -> "V";
			is LuaType.BuilderT  -> "V";
			is LuaType.StringT   -> "Ljava/lang/String;";
			is LuaType.CallbackT -> "L${LuaCallback::class.java.name.replace('.', '/')};";
			else                 -> "L${type.clazz.name.replace('.', '/')};";
		}
	}

	private fun typeToInt(type: LuaType): Int
	{
		return when (type)
		{
			is LuaType.DoubleT   -> LuaTypeId.Number.id;
			is LuaType.FloatT    -> LuaTypeId.Float.id;
			is LuaType.IntT      -> LuaTypeId.Int.id;
			is LuaType.BoolT     -> LuaTypeId.Bool.id;
			is LuaType.VoidT     -> LuaTypeId.Nil.id;
			is LuaType.StringT   -> LuaTypeId.String.id;
			is LuaType.Object    -> LuaTypeId.Table.id;
			is LuaType.BuilderT  -> LuaTypeId.Builder.id;
			is LuaType.CallbackT -> LuaTypeId.Function.id;
			else                 -> LuaTypeId.Nil.id;
		}
	}

	private fun stringToLuaType(type: String): LuaType
	{
		return when (type)
		{
			"int"                                            -> LuaType.Int
			"double"                                         -> LuaType.Double
			"float"                                          -> LuaType.Float
			"boolean"                                        -> LuaType.Bool
			"java.lang.string"                               -> LuaType.String
			"void"                                           -> LuaType.Void
			"com.minerkid08.dynamicopmodeloader.LuaCallback" -> LuaType.Callback
			else                                             -> LuaType.Object(Class.forName(type))
		}
	}
}