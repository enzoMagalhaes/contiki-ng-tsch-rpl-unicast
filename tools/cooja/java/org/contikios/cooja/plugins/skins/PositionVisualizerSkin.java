/*
 * Copyright (c) 2009, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

package org.contikios.cooja.plugins.skins;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import org.contikios.cooja.ClassDescription;
import org.contikios.cooja.Mote;
import org.contikios.cooja.Simulation;
import org.contikios.cooja.interfaces.Position;
import org.contikios.cooja.plugins.Visualizer;
import org.contikios.cooja.plugins.VisualizerSkin;

/**
 * Visualizer skin for mote positions.
 * <p>
 * Paints the three-dimensional mote position on the right-hand side of the mote.
 *
 * @author Fredrik Osterlind
 */
@ClassDescription("Positions")
public class PositionVisualizerSkin implements VisualizerSkin {
  private Simulation simulation;
  private Visualizer visualizer;

  @Override
  public void setActive(Simulation simulation, Visualizer vis) {
    this.simulation = simulation;
    this.visualizer = vis;
  }

  @Override
  public void setInactive() {
  }

  @Override
  public Color[] getColorOf(Mote mote) {
    return null;
  }

  @Override
  public void paintBeforeMotes(Graphics g) {
  }

  @Override
  public void paintAfterMotes(Graphics g) {
    g.setColor(Color.BLACK);

    /* Paint position coordinates right of motes */
    Mote[] allMotes = simulation.getMotes();
    for (Mote mote: allMotes) {
      Position pos = mote.getInterfaces().getPosition();
      Point pixel = visualizer.transformPositionToPixel(pos);

      String msg = "";
      String posString;
      String[] parts;

      /* X */
      posString = pos.getXCoordinate() + "000";
      parts = posString.split("\\.");
      if (parts[0].length() >= 4) {
        msg += parts[0];
      } else {
        msg += posString.substring(0, 5);
      }
      
      /* Y */
      msg += ", ";
      posString = pos.getYCoordinate() + "000";
      parts = posString.split("\\.");
      if (parts[0].length() >= 4) {
        msg += parts[0];
      } else {
        msg += posString.substring(0, 5);
      }

      /* Z */
      if (pos.getZCoordinate() != 0) {
        msg += ", ";
        posString = pos.getZCoordinate() + "000";
        parts = posString.split("\\.");
        if (parts[0].length() >= 4) {
          msg += parts[0];
        } else {
          msg += posString.substring(0, 5);
        }
      }
      
      g.drawString(msg, pixel.x + Visualizer.MOTE_RADIUS + 4, pixel.y + 4);
    }
  }

  @Override
  public Visualizer getVisualizer() {
    return visualizer;
  }
}
