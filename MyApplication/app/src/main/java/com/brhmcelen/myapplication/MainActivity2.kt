    package com.brhmcelen.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.AutoCompleteTextView
import android.widget.TextView
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.Response
import com.android.volley.toolbox.*

    class MainActivity2 : AppCompatActivity() {


        override fun onCreate(savedInstanceState: Bundle?) {
            super.onCreate(savedInstanceState)
            setContentView(R.layout.activity_main2)


        }


        fun getVolley() {
            val queue = Volley.newRequestQueue(this)

            val url: String = " https://api.thingspeak.com/channels/1346461/fields/1.json?api_key=4LRYMTHS44N3NPR8&results=2"
            val stringReq = StringRequest(Request.Method.GET, url, Response.Listener<String> { response ->

                var strResp = response.toString()

                Log.d("API", strResp)


            },
                    Response.ErrorListener { Log.d("API", "THAT DİDNT WORK") })

            queue.add((stringReq))


        }
    }