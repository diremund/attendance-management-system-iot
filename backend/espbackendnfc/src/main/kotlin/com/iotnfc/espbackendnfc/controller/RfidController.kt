package com.iotnfc.espbackendnfc.controller;


import org.springframework.beans.factory.annotation.Value
import org.springframework.http.HttpStatus
import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.PostMapping
import org.springframework.web.bind.annotation.RequestBody
import org.springframework.web.bind.annotation.RestController

@RestController
class RfidController {
    private val idList: ArrayList<String> = arrayListOf("GH2585", "IO2569", "TY5968")




    @PostMapping("/api/check-uid")
    fun checkUID(@RequestBody UIDresult: String): ResponseEntity<String> {
        // Assuming some validation logic here
        System.out.println("this is esp send: $UIDresult")
        if (isValidUID(UIDresult)) {
            return ResponseEntity.ok("Valid RFID Tag ID: $UIDresult")
        } else {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).body("Invalid RFID Tag ID")
        }
    }

    private fun isValidUID(UIDresult: String): Boolean {
        return true
    }



}
