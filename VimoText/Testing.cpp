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

    // Manager testing
    {
        std::cout << "Manager testing scenario:" << std::endl;
        PieceTable pt("Buenos dias a todos");
        CommandManager manager;

        std::cout << "-- After InsertTextCommand(19, \" al final\")" << std::endl;
        manager.executeCommand(
            std::make_unique<InsertTextCommand>(pt, 19, " al final")
        );
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [O:\"Buenos dias a todos\" -> A:\" al final\"]" << std::endl << std::endl;

        std::cout << "-- After undo()" << std::endl;
        manager.undo();
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [O:\"Buenos dias a todos\"]" << std::endl << std::endl;

        std::cout << "-- After redo()" << std::endl;
        manager.redo();
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [O:\"Buenos dias a todos\" -> A:\" al final\"]" << std::endl << std::endl;
    }

    // Test 1 - Empty + append
    {
        std::cout << "Test 1 - Empty + append:" << std::endl;
        PieceTable pt("");
        pt.InsertText("XYZ", 0);
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [A:\"XYZ\"]" << std::endl << std::endl;
    }

    // Test 2 - Insert and Remove + undo/redo
    {
        std::cout << "Test 2 - Insert and Remove + undo/redo:" << std::endl;
        PieceTable pt("ABCDEFGH");
        CommandManager manager;

        std::cout << "-- After RemoveTextCommand(2, 2)" << std::endl;
        manager.executeCommand(
            std::make_unique<RemoveTextCommand>(pt, 2, 2)
        );
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [O:\"AB\" -> O:\"EFGH\"]" << std::endl << std::endl;

        std::cout << "-- After undo()" << std::endl;
        manager.undo();
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [O:\"ABCDEFGH\"]" << std::endl << std::endl;

        std::cout << "-- After redo()" << std::endl;
        manager.redo();
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [O:\"AB\" -> O:\"EFGH\"]" << std::endl << std::endl;
    }

    // Test 3 - Mixed operations
    {
        std::cout << "Test 3 - Mixed operations:" << std::endl;
        PieceTable pt("12345");
        CommandManager manager;

        manager.executeCommand(std::make_unique<InsertTextCommand>(pt, 1, "A"));
        manager.executeCommand(std::make_unique<RemoveTextCommand>(pt, 3, 2));
        manager.executeCommand(std::make_unique<InsertTextCommand>(pt, 2, "XYZ"));
        pt.Dump(pt.GetSeqHead());

        std::cout << "[Esperado] [O:\"1A\" -> A:\"XYZ\" -> O:\"25\"]" << std::endl << std::endl;

        std::cout << "-- After undo x3" << std::endl;
        manager.undo();
        manager.undo();
        manager.undo();
        pt.Dump(pt.GetSeqHead());

        std::cout << "[Esperado] [O:\"12345\"]" << std::endl << std::endl;

        std::cout << "-- After redo x2" << std::endl;

        manager.redo();
        manager.redo();
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [O:\"1A\" -> O:\"25\"]" << std::endl << std::endl;

        std::cout << "-- After new InsertTextCommand(0, \"Z\")" << std::endl;
        manager.executeCommand(std::make_unique<InsertTextCommand>(pt, 0, "Z"));
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [A:\"Z\" -> O:\"1A\" -> O:\"25\"]" << std::endl << std::endl;

        std::cout << "-- After redo (should do nothing)" << std::endl;
        manager.redo();
        pt.Dump(pt.GetSeqHead());
        std::cout << "[Esperado] [A:\"Z\" -> O:\"1A\" -> O:\"25\"]" << std::endl << std::endl;
    }
}

