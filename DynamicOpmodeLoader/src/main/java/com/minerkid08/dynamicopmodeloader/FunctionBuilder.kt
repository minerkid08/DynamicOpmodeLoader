package com.minerkid08.dynamicopmodeloader

import java.lang.reflect.Method
import java.lang.reflect.Modifier

annotation class OpmodeLoaderBuilderFunction;
annotation class OpmodeLoaderFunction;


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
	external fun <T> setCurrentClass(thing: T);

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
			if (Modifier.isStatic(method.modifiers))
				continue;
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
				args.add(LuaType.fromStr(arg.name));
			addClassFunction(clazz, method.name, LuaType.fromStr(method.returnType.name), args);
		}
		for (method in toAddBuilder)
		{
			val args = ArrayList<LuaType>();
			for (arg in method.parameterTypes)
			{
				args.add(LuaType.fromStr(arg.name));
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
			if (Modifier.isStatic(method.modifiers))
				continue;
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
				args.add(LuaType.fromStr(arg.name));
			}
			addGlobalFunction(method.name, LuaType.fromStr(method.returnType.name), args);
		}
	}

	/**
	 * Takes a object and adds all of its static functions with the OpmodeLoaderFunction annotation to lua as global functions.
	 */
	fun addStaticClassAsGlobal(obj: Class<*>)
	{
		val toAdd = ArrayList<Method>();
		val methods = obj.methods;
		for (method in methods)
		{
			if (!Modifier.isStatic(method.modifiers))
				continue;
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

		setCurrentClass(obj);
		for (method in toAdd)
		{
			val args = ArrayList<LuaType>();
			for (arg in method.parameterTypes)
				args.add(LuaType.fromStr(arg.name));
			addStaticFunction(method.name, LuaType.fromStr(method.returnType.name), args);
		}
	}

	/**
	 * Adds a function from a java object to lua as a global.
	 */
	fun addGlobalFunction(
		name: String, rtnType: LuaType = LuaType.Void, argTypes: List<LuaType>? = null
	)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size ?: 0;
		addFunction(name, sig, LuaType.toInt(rtnType), argc);
	}

	/**
	 * Adds a static function from a java class to lua as a global.
	 */
	fun addStaticFunction(
		name: String, rtnType: LuaType = LuaType.Void, argTypes: List<LuaType>? = null
	)
	{
		val sig = generateSignature(rtnType, argTypes);
		val argc = argTypes?.size ?: 0;
		addFunctions(name, sig, LuaType.toInt(rtnType), argc);
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
		addFunctionc(clazz, name, sig, LuaType.toInt(rtnType), argc);
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
	private external fun addFunctions(name: String, funSignature: String, rtnType: Int, argc: Int);

	private external fun addFunctionc(
		clazz: Class<*>, name: String, funSignature: String, rtnType: Int, argc: Int
	);

	private fun generateSignature(rtnType: LuaType, argTypes: List<LuaType>?): String
	{
		var funSignature = "(";
		if (argTypes != null)
		{
			for (type in argTypes)
			{
				if (type is LuaType.VoidT) throw FunctionBuilderError("Void is not a valid argument type");
				if (type is LuaType.BuilderT) throw FunctionBuilderError("Builder is not a valid argument type");
				funSignature += LuaType.toStr(type);
			}
		}
		funSignature += ')';
		funSignature += LuaType.toStr(rtnType);

		funSignature = funSignature.replace('.', '/');

		return funSignature;
	}
}