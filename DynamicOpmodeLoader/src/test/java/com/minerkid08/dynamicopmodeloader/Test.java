package com.minerkid08.dynamicopmodeloader;

import java.util.List;

public class Test {
    void main()
    {
        OpmodeLoader opmodeLoader = new OpmodeLoader();
        var builder =  opmodeLoader.getFunctionBuilder();
        builder.addClassFunction(Test.class, "e", LuaType.Void, List.of(LuaType.Int, LuaType.Int));
    }
}
