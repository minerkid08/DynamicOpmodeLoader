package com.minerkid08.dynamicopmodeloader

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

abstract class LuaType(val clazz: Class<*>)
{
	class Object(clazz: Class<*>) : LuaType(clazz);
	class DoubleT : LuaType(kotlin.Double::class.java);
	class IntT : LuaType(kotlin.Int::class.java);
	class FloatT : LuaType(kotlin.Float::class.java);
	class BoolT : LuaType(Boolean::class.java);
	class StringT : LuaType(kotlin.String::class.java);
	class VoidT : LuaType(Any::class.java);
	class BuilderT : LuaType(Any::class.java);
	class CallbackT : LuaType(LuaCallback::class.java);

	companion object
	{
		@JvmField
		val Void = VoidT();

		@JvmField
		val Bool = BoolT();

		@JvmField
		val Double = DoubleT();

		@JvmField
		val Int = IntT();

		@JvmField
		val Float = FloatT();

		@JvmField
		val String = StringT();

		@JvmField
		val Builder = BuilderT();

		@JvmField
		val Callback = CallbackT();

		fun toStr(type: LuaType): String
		{
			return when (type)
			{
				is DoubleT   -> "D";
				is FloatT    -> "F";
				is IntT      -> "I";
				is BoolT     -> "Z";
				is VoidT     -> "V";
				is BuilderT  -> "V";
				is StringT   -> "Ljava/lang/String;";
				is CallbackT -> "L${LuaCallback::class.java.name.replace('.', '/')};";
				else         -> "L${type.clazz.name.replace('.', '/')};";
			}
		}

		fun toInt(type: LuaType): Int
		{
			return when (type)
			{
				is DoubleT   -> LuaTypeId.Number.id;
				is FloatT    -> LuaTypeId.Float.id;
				is IntT      -> LuaTypeId.Int.id;
				is BoolT     -> LuaTypeId.Bool.id;
				is VoidT     -> LuaTypeId.Nil.id;
				is StringT   -> LuaTypeId.String.id;
				is Object    -> LuaTypeId.Table.id;
				is BuilderT  -> LuaTypeId.Builder.id;
				is CallbackT -> LuaTypeId.Function.id;
				else         -> LuaTypeId.Nil.id;
			}
		}

		fun fromStr(type: String): LuaType
		{
			return when (type)
			{
				"int"                                            -> Int
				"double"                                         -> Double
				"float"                                          -> Float
				"boolean"                                        -> Bool
				"java.lang.String"                               -> String
				"void"                                           -> Void
				"com.minerkid08.dynamicopmodeloader.LuaCallback" -> Callback
				else                                             -> Object(Class.forName(type))
			}
		}
	}
}