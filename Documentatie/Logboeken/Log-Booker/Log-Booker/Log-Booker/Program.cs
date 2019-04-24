using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Compression;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System.Text.RegularExpressions;

namespace Stx.LogBooker
{
    public class Program
    {
        public static void Main(string[] args)
        {
            ConfigFile cf = new ConfigFile("test.cf");
        }
    }

    public class ConfigFile
    {
        public string Path { get; }
        public string FileAsString { get; private set; }

        public ConfigFile(string path, string createNewName = "Config File")
        {
            Path = path;

            if (!File.Exists(path))
                CreateNew(createNewName);

            FileAsString = File.ReadAllText(path);
        }

        public void CreateNew(string createNewName)
        {
            FileAsString = "* " + createNewName;
        }

        public void Save()
        {
            File.WriteAllText(Path, FileAsString);
        }

        public void Parse()
        {
            Regex r = new Regex(@"\[]")
        }
    }
}
