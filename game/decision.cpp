////////////////////////////////////////////////////////////////////////////////
//
//    Decision()
//
////////////////////////////////////////////////////////////////////////////////
int atime[8] = {400, 0, 0, 0, 200, 400, 600, 700};

void Attack() {
  // wyslac zolnierzy o numerach>2  kaplanow o nr>1 i wodzow
  mem.delay++;
  if (debug) {
    if (mem.delay < 1000)
      sprintf(Msg.msg, "%d", 1000 - mem.delay);
    else
      sprintf(Msg.msg, "Atak!");
    Msg.licznik = 10;
  }
  if (mem.delay > 1100) {
    if (mem.atak < 7)
      mem.atak++;
    else
      mem.atak = 0;
    mem.delay = atime[mem.atak];
    return;
  }
  if (mem.delay > 1000) // jezeli nie mina zadany czas
  {
    if (mem.aindex > 19) {
      mem.aindex = 0;
      return;
    }
    if (!mem.c->b[mem.aindex].exist) {
      mem.aindex++;
      return;
    } // brak budynku
    if (mem.c->b[mem.aindex].type < 3) {
      mem.aindex++;
      return;
    }
    for (int i = 0; i < 6; i++) {
      if (i < 2 || i == 3 || i == 5) {
        int x, y;
        x = mem.c->b[mem.aindex].m[i].x;
        y = mem.c->b[mem.aindex].m[i].y;
        mem.distance = 0;
        FindEnemy(x, y, &x, &y, &mem.distance);
        if (x != 32 || y != 32) {
          mem.c->b[mem.aindex].m[i].SetEnd(x, y);
          mem.c->b[mem.aindex].m[i].SetCommand(1);
        }
        if (x == 32 && y == 32) {
          if (mem.c->b[mem.aindex].m[i].type == 1) {
            mem.c->b[mem.aindex].m[i].SetCommand(11);
          } else {
            if (mem.c->b[mem.aindex].m[i].type == 4) {
              mem.distance = 2;
              x = mem.c->b[mem.aindex].m[i].x;
              y = mem.c->b[mem.aindex].m[i].y;
              FindEnemy(x, y, &x, &y, &mem.distance);
              if (x != 32 || y != 32) {
                mem.c->b[mem.aindex].m[i].SetCommand(2); // rozwal palisade
                mem.c->b[mem.aindex].m[i].SetTarget(place[x][y]);
              }
            }
          }
        }
      }
    }
    mem.aindex++;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//    Decision
//
////////////////////////////////////////////////////////////////////////////////
void Decision() {
  int i;
  int position[4][2] = {{-1, -1}, {3, -1}, {3, 3}, {-1, 2}};
  if (mem.faza == 0) // inicjalizacja
  {
    mem.dmilk = diff;
    mem.faza = 1;
    mem.c = &castle[1];
    mem.index = 0;
    mem.aindex = 0;
    mem.atak = 0;
    mem.delay = atime[mem.atak];
    return;
  }
  //-----przyrost mleka------
  mem.c->milk += mem.dmilk;
  //-----odbudowa-----------------------
  for (i = 0; i < 20; i++) {
    if (mem.c->b[i].exist == 1 && mem.c->b[i].hp &&
        mem.c->b[i].hp < mem.c->b[i].maxhp) {
      int xx, yy, k = 0;

      FindEnemy(castle[1].b[i].x, castle[1].b[i].y, &xx, &yy, &k);
      for (int j = 0; j < 6; j++) {
        if (castle[1].m[j + 1].type == 1 &&
            castle[1].m[j + 1].command != 8) { // ekipa remontowa
          castle[1].m[j + 1].SetEnd(castle[1].b[i].x + (j & 2),
                                    castle[1].b[i].y);
          castle[1].m[j + 1].SetCommand(8);
        }
        if (castle[1].m[j + 1].type > 1 && !castle[1].m[j + 1].command &&
            (j > 1)) { // straznicy
          castle[1].m[j + 1].SetEnd(xx, yy);
          castle[1].m[j + 1].SetCommand(1);
        }
        if (castle[1].b[i].m[j].type == 1 &&
            castle[1].b[i].m[j].command != 8) { // wlasna ekipa remontowa
          castle[1].b[i].m[j].SetEnd(castle[1].b[i].x + (j & 2),
                                     castle[1].b[i].y);
          castle[1].b[i].m[j].SetCommand(8);
        }
        if (castle[1].b[i].m[j].type > 1 && !castle[1].b[i].m[j].command &&
            !k) { // wlasni straznicy
          castle[1].b[i].m[j].SetTarget(place[xx][yy]);
          castle[1].b[i].m[j].SetCommand(2);
        }
      }
    }
  }
  Attack();

  if (mem.faza == 1) // budowa krow
  {
    if (!mem.index) {
      int j = 0;
      for (i = 0; i < 19; i++) {
        if (mem.c->b[i].type == 2) // obora
        {
          for (int k = 0; k < 3; k++) {
            if (mem.c->b[i].m[k].type == 0 && mem.c->b[i].m[k].exist == 1)
              j++;
          }
        }
      }
      if ((!j) && mem.c->milk < 500)
        mem.c->milk = 500;
    }
    char typ;
    if (mem.index > 19) {
      mem.index = 0;
      mem.faza = 11;
      return;
    } // koniec produkcji krow

    if (mem.c->b[mem.index].exist != 1 || mem.c->b[mem.index].type != 2) {
      mem.index++;
      return;
    } // budynek w zlym stanie

    if (mem.c->milk < 450)
      return;
    if ((mem.c->b[mem.index].type == 2) &&
        (mem.c->b[mem.index].food !=
         2)) // tylko obore gdzie mniej niz 2 krowy biezemy pod uwage
    {
      typ = 2;
      Cmd[1].co = 0;
      Cmd[1].nrb = mem.index;
      Cmd[1].command = typ;
    }
    mem.index++;
    return;
  }

  if (mem.faza == 11) // budowa   postaci
  {
    char typ;
    if (mem.index > 19) {
      mem.index = 0;
      mem.faza = 2;
      return;
    }
    if (mem.c->b[mem.index].exist != 1) {
      mem.index++;
      return;
    } // budynek w zlym stanie
    if (mem.c->milk < 1250)
      return;
    switch (mem.c->b[mem.index].type) {
    case 1:
      typ = 1;
      break; //???????? droga????
    case 2:
      typ = 2;
      if (level > PASTUCH_LEV && mem.c->b[mem.index].food == 2) {
        typ = 6;
        mem.c->milk += 200 * diff;
      }
      break; // krowa,pastuch
    case 3:
      typ = 2 + (mouse.X & 1);
      break;
    case 4:
      typ = 3 + (mouse.X & 2);
      if (level > MAG_LEV && mem.c->b[mem.index].food == 2) {
        typ = 6;
        mem.c->milk += 300;
      }
      break; // kaplanka, kaplan, mag
    case 5:
      typ = 3 + (mouse.X & 1);
      break; // woj, wlucznik
    case 6:
      typ = 5 + (mouse.X & 1);
      break; // rycerz, kusznik
    }
    if (mem.c->b[mem.index].type > 1) {
      Cmd[1].co = 0;
      Cmd[1].nrb = mem.index;
      Cmd[1].command = typ;
    }
    mem.index++;
    return;
  }

  //-======================================================================
  if (mem.faza == 2) // wyslanie postaci
  {
    if (mem.index > 19) {
      mem.index = 0;
      mem.faza = 3;
      return;
    }

    if (!mem.c->b[mem.index].exist) {
      mem.index++;
      return;
    } // brak budynku
    if (mem.c->b[mem.index].type == 1) {
      mem.index++;
      return;
    }                                  // glowny
    if (mem.c->b[mem.index].type == 2) // obora
    {
      int xt, yt;

      for (i = 0; i < 3; i++) {
        FindGrass(mem.xp, mem.yp, &xt, &yt);
        mem.c->b[mem.index].m[i].SetEnd(xt, yt);
        mem.c->b[mem.index].m[i].SetCommand(1);
      }
      mem.index++;
      return;
    }
    if (mem.c->b[mem.index].type > 2) // koszary swiatynie itp.
    {
      int xt, yt;
      for (i = 0; i < 6; i++) {
        if (!mem.c->b[mem.index].m[i].command &&
            (mem.c->b[mem.index].m[i].hp > 5)) {
          if (i == 1 || i == 3 || i == 5) {
            xt = mem.xz;
            yt = mem.yz;
          } else {
            xt = mem.c->b[mem.index].x + position[i / 2][0];
            yt = mem.c->b[mem.index].y + position[i / 2][1];
          }
          mem.c->b[mem.index].m[i].SetEnd(xt, yt);
          mem.c->b[mem.index].m[i].SetCommand(1);
        }
      }
    }
    mem.index++;
    return;
  }
  //////////////////////////////////////////////////////////////
  if (mem.faza == 3) // powrot
  {
    mem.index = 0;
    mem.faza = 1;
    return;
  }
}
