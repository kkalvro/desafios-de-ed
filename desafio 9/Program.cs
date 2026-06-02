using System;
using System.Collections.Generic;

class Program
{
    static Dictionary<string, List<string>> mapa =
        new Dictionary<string, List<string>>();

    static void Main()
    {
        CriarMapa();

        while (true)
        {
            Console.WriteLine("\nMenu:");
            Console.WriteLine("1) Listar cidades");
            Console.WriteLine("2) Conexão direta");
            Console.WriteLine("3) Existe rota? (DFS)");
            Console.WriteLine("4) Menor rota (BFS)");
            Console.WriteLine("5) Sair");
            Console.Write("Opção: ");

            string opcao = Console.ReadLine();

            switch (opcao)
            {
                case "1":
                    ListarCidades();
                    break;

                case "2":
                    ConexaoDireta();
                    break;

                case "3":
                    ExisteRotaDFS();
                    break;

                case "4":
                    MenorRotaBFS();
                    break;

                case "5":
                    return;

                default:
                    Console.WriteLine("Opção inválida.");
                    break;
            }
        }
    }

    static void CriarMapa()
    {
        mapa["São Paulo"] = new List<string>()
        {
            "Rio de Janeiro",
            "Curitiba",
            "Belo Horizonte"
        };

        mapa["Rio de Janeiro"] = new List<string>()
        {
            "São Paulo",
            "Belo Horizonte",
            "Vitória"
        };

        mapa["Belo Horizonte"] = new List<string>()
        {
            "São Paulo",
            "Rio de Janeiro",
            "Brasília"
        };

        mapa["Curitiba"] = new List<string>()
        {
            "São Paulo",
            "Florianópolis"
        };

        mapa["Florianópolis"] = new List<string>()
        {
            "Curitiba",
            "Porto Alegre"
        };

        mapa["Porto Alegre"] = new List<string>()
        {
            "Florianópolis"
        };

        mapa["Brasília"] = new List<string>()
        {
            "Belo Horizonte",
            "Goiânia"
        };

        mapa["Goiânia"] = new List<string>()
        {
            "Brasília"
        };

        mapa["Vitória"] = new List<string>()
        {
            "Rio de Janeiro"
        };

        mapa["Salvador"] = new List<string>()
        {
            "Recife"
        };

        mapa["Recife"] = new List<string>()
        {
            "Salvador",
            "Fortaleza"
        };

        mapa["Fortaleza"] = new List<string>()
        {
            "Recife"
        };
    }

    static void ListarCidades()
    {
        Console.WriteLine();

        foreach (string cidade in mapa.Keys)
        {
            Console.Write(cidade + ": ");

            foreach (string vizinho in mapa[cidade])
            {
                Console.Write(vizinho + " ");
            }

            Console.WriteLine();
        }
    }

    static void ConexaoDireta()
    {
        Console.Write("Origem: ");
        string origem = Console.ReadLine();

        Console.Write("Destino: ");
        string destino = Console.ReadLine();

        if (mapa.ContainsKey(origem) &&
            mapa[origem].Contains(destino))
        {
            Console.WriteLine("Existe conexão direta.");
        }
        else
        {
            Console.WriteLine("Não existe conexão direta.");
        }
    }

    static void ExisteRotaDFS()
    {
        Console.Write("Origem: ");
        string origem = Console.ReadLine();

        Console.Write("Destino: ");
        string destino = Console.ReadLine();

        HashSet<string> visitados =
            new HashSet<string>();

        bool existe =
            DFS(origem, destino, visitados);

        if (existe)
            Console.WriteLine("Existe rota.");
        else
            Console.WriteLine("Não existe rota.");
    }

    static bool DFS(
        string atual,
        string destino,
        HashSet<string> visitados)
    {
        if (atual == destino)
            return true;

        visitados.Add(atual);

        foreach (string vizinho in mapa[atual])
        {
            if (!visitados.Contains(vizinho))
            {
                if (DFS(vizinho, destino, visitados))
                    return true;
            }
        }

        return false;
    }

    static void MenorRotaBFS()
    {
        Console.Write("Origem: ");
        string origem = Console.ReadLine();

        Console.Write("Destino: ");
        string destino = Console.ReadLine();

        Queue<string> fila =
            new Queue<string>();

        HashSet<string> visitados =
            new HashSet<string>();

        Dictionary<string, string> anterior =
            new Dictionary<string, string>();

        fila.Enqueue(origem);
        visitados.Add(origem);

        bool encontrou = false;

        while (fila.Count > 0)
        {
            string atual = fila.Dequeue();

            if (atual == destino)
            {
                encontrou = true;
                break;
            }

            foreach (string vizinho in mapa[atual])
            {
                if (!visitados.Contains(vizinho))
                {
                    visitados.Add(vizinho);
                    fila.Enqueue(vizinho);

                    anterior[vizinho] = atual;
                }
            }
        }

        if (!encontrou)
        {
            Console.WriteLine("Não existe rota.");
            return;
        }

        List<string> caminho =
            new List<string>();

        string cidadeAtual = destino;

        while (cidadeAtual != origem)
        {
            caminho.Add(cidadeAtual);
            cidadeAtual = anterior[cidadeAtual];
        }

        caminho.Add(origem);

        caminho.Reverse();

        Console.WriteLine("\nMenor rota:");

        foreach (string cidade in caminho)
        {
            Console.Write(cidade);

            if (cidade != caminho[caminho.Count - 1])
                Console.Write(" -> ");
        }

        Console.WriteLine();
        Console.WriteLine("Paradas: " + (caminho.Count - 1));
    }
}