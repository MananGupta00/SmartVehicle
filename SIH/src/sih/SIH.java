/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sih;
 

/**
 *
 * @author manan
 */
public class SIH  {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
         Wireless arduino = new Wireless();
        arduino.initialize();
        Thread t=new Thread() {
            public void run() {
                //the following line will keep this app alive for 10 seconds,
                //waiting for events to occur and responding to them (printing incoming messages to console).
                try {Thread.sleep(10000);
                System.out.println("time");}
                catch (InterruptedException ie) {}
            }
        };
        t.start();
        System.out.println("Started");
        
    }
    
}
