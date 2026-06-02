using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        Dictionary<string, int> frequencias = new Dictionary<string, int>();

        while (true)
        {
            Console.WriteLine();
            Console.WriteLine("Menu:");
            Console.WriteLine("1) Novo texto");
            Console.WriteLine("2) Buscar palavra");
            Console.WriteLine("3) Comparar textos");
            Console.WriteLine("4) Sair");
            Console.Write("Escolha: ");

            string opcao = Console.ReadLine();

            switch (opcao)
            {
                case "1":
                    frequencias = LerTexto();
                    MostrarEstatisticas(frequencias);
                    break;

                case "2":
                    BuscarPalavra(frequencias);
                    break;

                case "3":
                    CompararTextos();
                    break;

                case "4":
                    return;

                default:
                    Console.WriteLine("Opcao invalida.");
                    break;
            }
        }
    }

    static Dictionary<string, int> LerTexto()
    {
        Console.WriteLine("Digite o texto (linha vazia para encerrar):");

        string texto = "";

        while (true)
        {
            string linha = Console.ReadLine();

            if (linha == "")
                break;

            texto += " " + linha;
        }

        texto = texto.ToLower();

        string textoLimpo = "";

        foreach (char c in texto)
        {
            if (char.IsLetterOrDigit(c) || char.IsWhiteSpace(c))
                textoLimpo += c;
        }

        string[] palavras = textoLimpo.Split(
            new char[] { ' ', '\t', '\n', '\r' },
            StringSplitOptions.RemoveEmptyEntries);

        Dictionary<string, int> frequencias = new Dictionary<string, int>();

        foreach (string palavra in palavras)
        {
            if (frequencias.ContainsKey(palavra))
                frequencias[palavra]++;
            else
                frequencias.Add(palavra, 1);
        }

        return frequencias;
    }

    static void MostrarEstatisticas(Dictionary<string, int> frequencias)
    {
        int totalPalavras = 0;

        foreach (int qtd in frequencias.Values)
            totalPalavras += qtd;

        Console.WriteLine();
        Console.WriteLine("Resultado:");
        Console.WriteLine("Total de palavras: " + totalPalavras);
        Console.WriteLine("Palavras distintas: " + frequencias.Count);

        List<KeyValuePair<string, int>> lista =
            new List<KeyValuePair<string, int>>();

        foreach (KeyValuePair<string, int> item in frequencias)
            lista.Add(item);

        for (int i = 0; i < lista.Count - 1; i++)
        {
            for (int j = i + 1; j < lista.Count; j++)
            {
                if (lista[j].Value > lista[i].Value)
                {
                    KeyValuePair<string, int> aux = lista[i];
                    lista[i] = lista[j];
                    lista[j] = aux;
                }
            }
        }

        Console.WriteLine();
        Console.WriteLine("Top 10 palavras mais frequentes:");

        int limite = 10;

        if (lista.Count < 10)
            limite = lista.Count;

        for (int i = 0; i < limite; i++)
        {
            Console.WriteLine(
                (i + 1) + ". \"" +
                lista[i].Key +
                "\" -> " +
                lista[i].Value +
                " ocorrencia(s)");
        }
    }

    static void BuscarPalavra(Dictionary<string, int> frequencias)
    {
        if (frequencias.Count == 0)
        {
            Console.WriteLine("Nenhum texto carregado.");
            return;
        }

        Console.Write("Qual palavra deseja buscar? ");

        string palavra = Console.ReadLine().ToLower();

        if (frequencias.ContainsKey(palavra))
            Console.WriteLine("Aparece " + frequencias[palavra] + " vez(es).");
        else
            Console.WriteLine("Palavra nao encontrada.");
    }

    static void CompararTextos()
    {
        Console.WriteLine();
        Console.WriteLine("Texto 1:");

        Dictionary<string, int> texto1 = LerTexto();

        Console.WriteLine();
        Console.WriteLine("Texto 2:");

        Dictionary<string, int> texto2 = LerTexto();

        Console.WriteLine();
        Console.WriteLine("Palavras em comum:");

        bool encontrou = false;

        foreach (string palavra in texto1.Keys)
        {
            if (texto2.ContainsKey(palavra))
            {
                Console.WriteLine(palavra);
                encontrou = true;
            }
        }

        if (!encontrou)
            Console.WriteLine("Nenhuma palavra em comum.");
    }
}
