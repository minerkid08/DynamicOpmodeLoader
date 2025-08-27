package com.minerkid08.dynamicopmodeloader;

import org.junit.Test;

public class JMain {
    static class JE
    {
        @OpmodeLoaderFunction
        public void doThing(LuaCallback callback)
        {
            System.out.println("did thing");
            callback.call(69);
            System.out.println("called callback");
        }

        @OpmodeLoaderFunction
        public double add(int a, double b)
        {
            return a + b;
        }

        @OpmodeLoaderBuilderFunction
        public void printThing(String a)
        {
            System.out.printf("printed thing %s\n", a);
        }
    }

    static class JF
    {
        @OpmodeLoaderFunction
        public static JE getE()
        {
            return new JE();
        }
    }

    @Test
    public void main() {
        OpmodeLoader opmodeLoader = new OpmodeLoader();

        FunctionBuilder builder = opmodeLoader.getFunctionBuilder();

        builder.addClassAsClass(JE.class);
        builder.addStaticClassAsGlobal(JF.class);

        opmodeLoader.init();
        opmodeLoader.loadOpmode(":)");
        opmodeLoader.start(1);
        opmodeLoader.update(0.0, 0.0);
        opmodeLoader.close();
    }
}
