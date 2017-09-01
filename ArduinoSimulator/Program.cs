using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http.Headers;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ArduinoSimulator {
    class Program {
        static void Main(string[] args) {
            var web = new HttpListener();
            web.Prefixes.Add("http://*:8090/");
            web.Start();

            while (true) {
                var context = web.GetContext();
                var response = context.Response;
                Console.WriteLine("Passsed here");
                string responseString = GetResponse(context);
                var buffer = System.Text.Encoding.UTF8.GetBytes(responseString);
                response.ContentLength64 = buffer.Length;
                var output = response.OutputStream;
                output.Write(buffer, 0, buffer.Length);
                output.Close();
            }
            
        }

        public static string GetResponse(HttpListenerContext context) {
            
            string response = "false";

            Console.Write(context.Request.QueryString["request"] + " response? ");


            return Console.ReadLine();
        }
    }
}