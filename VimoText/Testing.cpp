#include "Testing.h"


void testing() {
    // 1) Lista vacía + append puro
    {
        std::cout << "Test 1 - Vacío + append:" << std::endl;
        PieceTable pt("");
        pt.InsertText("XYZ", 0);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] XYZ" << std::endl << std::endl;
    }

    // 2) Insert al inicio (firstHalf = 0)
    {
        std::cout << "Test 2 - Insert al inicio:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.InsertText("X", 0);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] XABCDEF" << std::endl << std::endl;
    }

    // 3) Insert al final
    {
        std::cout << "Test 3 - Insert al final:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.InsertText("X", 6);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABCDEF X" << std::endl << std::endl;
    }

    // 4) Insert en medio (split pieza original)
    {
        std::cout << "Test 4 - Insert en medio:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.InsertText("X", 3);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABCXDEF" << std::endl << std::endl;
    }

    // 5) Insert dentro de append (split append)
    {
        std::cout << "Test 5 - Insert dentro de append:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.InsertText("X", 3);  // ABCXDEF
        pt.InsertText("Y", 4);  // ABCXYDEF
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABCXYDEF" << std::endl << std::endl;
    }

    // 6) Insert al final exacto
    {
        std::cout << "Test 6 - Insert al final exacto:" << std::endl;
        PieceTable pt("ABCDE");
        pt.InsertText("Z", 5);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABCDEZ" << std::endl << std::endl;
    }

    // 7) Split genérico
    {
        std::cout << "Test 7 - Split genérico:" << std::endl;
        PieceTable pt("12345");
        pt.InsertText("+++", 2);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] 12+++345" << std::endl << std::endl;
    }

    // 8) Append tras split
    {
        std::cout << "Test 8 - Append tras split:" << std::endl;
        PieceTable pt("12345");
        pt.InsertText("+++", 2);  // 12+++345
        pt.InsertText("END", 8);  // 12+++345END
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] 12+++345END" << std::endl << std::endl;
    }

    // REMOVING
    // 1) Eliminar en lista vacía (no debe hacer nada)
    {
        std::cout << "Test 1 - Remove en vacío:" << std::endl;
        PieceTable pt("");
        pt.RemoveText(0, 1);  // Intento eliminar en posición inexistente
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] " << std::endl << std::endl;  // Nada
    }

    // 2) Eliminar al inicio de pieza única
    {
        std::cout << "Test 2 - Remove al inicio:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.RemoveText(0, 1);  // Elimina 'A'
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] BCDEF" << std::endl << std::endl;
    }

    // 3) Eliminar al final de pieza única
    {
        std::cout << "Test 3 - Remove al final:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.RemoveText(5, 1);  // Elimina 'F'
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABCDE" << std::endl << std::endl;
    }

    // 4) Eliminar en medio de pieza única (split)
    {
        std::cout << "Test 4 - Remove en medio:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.RemoveText(2, 2);  // Elimina 'CD'
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABEF" << std::endl << std::endl;
    }

    // 5) Eliminar abarcando múltiples piezas
    {
        std::cout << "Test 5 - Remove en múltiples piezas:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.InsertText("XYZ", 3);  // ABCXYZDEF
        pt.Dump(pt.GetSeqHead());
        pt.RemoveText(2, 5);      // Elimina 'C', 'XYZ', 'D' ? AB EF
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABEF" << std::endl << std::endl;
    }

    // 6) Eliminar pieza completa
    {
        std::cout << "Test 6 - Remove pieza completa:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.InsertText("XYZ", 3);  // ABCXYZDEF
        pt.RemoveText(3, 3);      // Elimina 'XYZ' ? ABC DEF
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABCDEF" << std::endl << std::endl;
    }

    // 7) Eliminar en límite exacto de piezas
    {
        std::cout << "Test 7 - Remove en límite:" << std::endl;
        PieceTable pt("ABCDEF");
        pt.InsertText("XYZ", 3);  // ABC XYZ DEF
        pt.RemoveText(3, 0);      // No-op
        pt.RemoveText(6, 3);      // Elimina 'DEF' ? ABC XYZ
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABCXYZ" << std::endl << std::endl;
    }

    // 8) Eliminar múltiples piezas completas
    {
        std::cout << "Test 8 - Remove múltiples piezas:" << std::endl;
        PieceTable pt("");
        pt.InsertText("A", 0);
        pt.InsertText("B", 1);
        pt.InsertText("C", 2);  // Piezas: A | B | C
        pt.RemoveText(0, 3);    // Elimina todo
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] " << std::endl << std::endl;  // Nada
    }

    // 9) Eliminar rango que empieza antes y acaba dentro de la misma pieza
    {
        std::cout << "Test 9 - Remove rango parcial en pieza:" << std::endl;
        PieceTable pt("ABCDEFGH");
        // Elimina 'CDE' (índice 2, longitud 3) → ABFGH
        pt.RemoveText(2, 3);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] ABFGH" << std::endl << std::endl;
    }

    // 10) Eliminar texto que cruza el punto de inserción de un Insert previo
    {
        std::cout << "Test 10 - Remove cruzando inserción previa:" << std::endl;
        PieceTable pt("123456789");
        pt.InsertText("ABC", 4);   // 1234ABC56789
        // Elimina desde pos 2 longitud 7 → elimina '34ABC56' → 12789
        pt.RemoveText(2, 7);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] 12789" << std::endl << std::endl;
    }

    // 11) Eliminar más allá del final (should clamp)
    {
        std::cout << "Test 11 - Remove más allá del final:" << std::endl;
        PieceTable pt("WXYZ");
        // Longitud excesiva; debería eliminar hasta el final → ''
        pt.RemoveText(1, 100);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] W" << std::endl << std::endl;
    }

    // 12) Remove con longitud cero en medio de texto
    {
        std::cout << "Test 12 - Remove longitud 0:" << std::endl;
        PieceTable pt("HELLO");
        pt.RemoveText(2, 0);  // no-op
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] HELLO" << std::endl << std::endl;
    }

    // 13) Remove en posición negativa o invalida (debería no hacer nada o lanzar)
    {
        std::cout << "Test 13 - Remove posición negativa:" << std::endl;
        PieceTable pt("TESTING");
        pt.RemoveText(-5, 3);  // define comportamiento: clamp a 0 o no-op
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] TESTING" << std::endl << std::endl;
    }

    // 14) Secuencia de múltiples removes encadenados
    {
        std::cout << "Test 14 - Multiple removes secuenciales:" << std::endl;
        PieceTable pt("ABCDEFGHIJ");
        pt.RemoveText(2, 3);  // ABFGHIJ
        pt.RemoveText(4, 2);  // ABFGJ
        pt.RemoveText(1, 1);  // AFGJ
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] AFGJ" << std::endl << std::endl;
    }

    // 15) Insert intercalado y luego remove en áreas no contiguas
    {
        std::cout << "Test 15 - Remove en áreas intercaladas:" << std::endl;
        PieceTable pt("XXXX");
        pt.InsertText("111", 2);  // XX111XX
        pt.InsertText("222", 5);  // XX111222XX
        pt.RemoveText(2, 2);      // XX1222XX
        pt.DumpReverse(pt.GetSeqHead());
        pt.Dump(pt.GetSeqHead());
        pt.RemoveText(4, 1);      // XX122XX
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] XX122XX" << std::endl << std::endl;
    }

    // 16) Remove completo tras varias inserciones y eliminaciones
    {
        std::cout << "Test 16 - Remove todo tras operaciones mixtas:" << std::endl;
        PieceTable pt("");
        pt.InsertText("A", 0);
        pt.Dump(pt.GetSeqHead());
        pt.DumpReverse(pt.GetSeqHead());

        pt.InsertText("B", 1);
        pt.InsertText("C", 2);
        pt.InsertText("D", 3);      // ABCD
        pt.Dump(pt.GetSeqHead());
        pt.DumpReverse(pt.GetSeqHead());

        pt.RemoveText(1, 2);        // A D
        pt.Dump(pt.GetSeqHead());
        pt.DumpReverse(pt.GetSeqHead());

        pt.InsertText("XYZ", 1);    // AXYZD
        pt.Dump(pt.GetSeqHead());
        pt.DumpReverse(pt.GetSeqHead());

        pt.RemoveText(0, 5);        // ''
        pt.Dump(pt.GetSeqHead());
        pt.DumpReverse(pt.GetSeqHead());
        std::cout << "[Esperado] " << std::endl;
    }

    // 17) Remove en texto muy largo (performance / integridad)
    {
        std::cout << "Test 17 - Remove en texto largo:" << std::endl;
        std::string longText(1000, 'X');
        PieceTable pt(longText);
        pt.RemoveText(450, 100);     // elimina en medio
        auto head = pt.GetSeqHead();
        pt.Dump(head);
        std::cout << "[Esperado] 450 X's + 450 X's" << std::endl << std::endl;
    }

    // 18) Remove que une dos piezas resultando en merge
    {
        std::cout << "Test 18 - Remove que causa merge de piezas:" << std::endl;
        PieceTable pt("HELLO");
        pt.InsertText("123", 2);   // HE123LLO
        // Al eliminar '123', las piezas "HE" y "LLO" deben fusionarse
        pt.RemoveText(2, 3);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] HELLO" << std::endl << std::endl;
    }

    // Manager testing
    //{
    //    std::cout << "Manager testing scenario:" << std::endl;
    //    PieceTable pt("Buenos dias a todos");
    //    CommandManager manager;

    //    std::cout << "-- After InsertTextCommand(19, \" al final\")" << std::endl;
    //    manager.executeCommand(
    //        std::make_unique<InsertTextCommand>(pt, 19, " al final")
    //    );
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [O:\"Buenos dias a todos\" -> A:\" al final\"]" << std::endl << std::endl;

    //    std::cout << "-- After undo()" << std::endl;
    //    manager.undo();
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [O:\"Buenos dias a todos\"]" << std::endl << std::endl;

    //    std::cout << "-- After redo()" << std::endl;
    //    manager.redo();
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [O:\"Buenos dias a todos\" -> A:\" al final\"]" << std::endl << std::endl;
    //}

    //// Test 1 - Empty + append
    //{
    //    std::cout << "Test 1 - Empty + append:" << std::endl;
    //    PieceTable pt("");
    //    pt.InsertText("XYZ", 0);
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [A:\"XYZ\"]" << std::endl << std::endl;
    //}

    //// Test 2 - Insert and Remove + undo/redo
    //{
    //    std::cout << "Test 2 - Insert and Remove + undo/redo:" << std::endl;
    //    PieceTable pt("ABCDEFGH");
    //    CommandManager manager;

    //    std::cout << "-- After RemoveTextCommand(2, 2)" << std::endl;
    //    manager.executeCommand(
    //        std::make_unique<RemoveTextCommand>(pt, 2, 2)
    //    );
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [O:\"AB\" -> O:\"EFGH\"]" << std::endl << std::endl;

    //    std::cout << "-- After undo()" << std::endl;
    //    manager.undo();
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [O:\"ABCDEFGH\"]" << std::endl << std::endl;

    //    std::cout << "-- After redo()" << std::endl;
    //    manager.redo();
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [O:\"AB\" -> O:\"EFGH\"]" << std::endl << std::endl;
    //}

    //// Test 3 - Mixed operations
    //{
    //    std::cout << "Test 3 - Mixed operations:" << std::endl;
    //    PieceTable pt("12345");
    //    CommandManager manager;

    //    manager.executeCommand(std::make_unique<InsertTextCommand>(pt, 1, "A"));
    //    manager.executeCommand(std::make_unique<RemoveTextCommand>(pt, 3, 2));
    //    manager.executeCommand(std::make_unique<InsertTextCommand>(pt, 2, "XYZ"));
    //    pt.Dump(pt.GetSeqHead());

    //    std::cout << "[Esperado] [O:\"1A\" -> A:\"XYZ\" -> O:\"25\"]" << std::endl << std::endl;

    //    std::cout << "-- After undo x3" << std::endl;
    //    manager.undo();
    //    manager.undo();
    //    manager.undo();
    //    pt.Dump(pt.GetSeqHead());

    //    std::cout << "[Esperado] [O:\"12345\"]" << std::endl << std::endl;

    //    std::cout << "-- After redo x2" << std::endl;

    //    manager.redo();
    //    manager.redo();
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [O:\"1A\" -> O:\"25\"]" << std::endl << std::endl;

    //    std::cout << "-- After new InsertTextCommand(0, \"Z\")" << std::endl;
    //    manager.executeCommand(std::make_unique<InsertTextCommand>(pt, 0, "Z"));
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [A:\"Z\" -> O:\"1A\" -> O:\"25\"]" << std::endl << std::endl;

    //    std::cout << "-- After redo (should do nothing)" << std::endl;
    //    manager.redo();
    //    pt.Dump(pt.GetSeqHead());
    //    std::cout << "[Esperado] [A:\"Z\" -> O:\"1A\" -> O:\"25\"]" << std::endl << std::endl;
    //}
}

