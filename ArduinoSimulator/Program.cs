using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Net;
using System.Net.Http.Headers;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ArduinoSimulator {
    class Program {

        static Random rnd = new Random();
        private float temp = 16.5f;


        static void Main(string[] args) {
            var web = new HttpListener();
            web.Prefixes.Add("http://*:8090/");
            web.Start();

            while (true) {
                var context = web.GetContext();
                var response = context.Response;
                string responseString = GetResponse(context);
                var buffer = System.Text.Encoding.UTF8.GetBytes(responseString);
                response.ContentLength64 = buffer.Length;
                var output = response.OutputStream;
                output.Write(buffer, 0, buffer.Length);
                output.Close();
            }
            
        }

        public static string GetResponse(HttpListenerContext context) {
            
            string request = context.Request.QueryString["request"];

            Console.WriteLine("Requested " + request);

            if (request == "getTemp")
                return ((rnd.NextDouble() + 1) * 15).ToString(CultureInfo.InvariantCulture);

            if (request == "setOn" || request == "setOff" || request == "setFOn")
                return "true";

            if (request == "ping") return "true";

            if (request == "getState") return rnd.Next(0, 2) == 0 ? "On" : "Off";

            if (request == "getDeviceInfo") return "Arduino Simulator";

            if (request == "getHumi")
                return rnd.Next(0,101).ToString(CultureInfo.InvariantCulture);

            if (request == "setTemp") {
                Console.WriteLine("temp: " + context.Request.QueryString["p0"]);
                return "true";
            }

            if (request == "getHIndex")
                return "diocan g'ho minga voglia di calcolarlo a man";

            Console.WriteLine("Command not recognized, enter respose: ");
            return Console.ReadLine();
        }
    }
}