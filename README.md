
# Arduino Temperature Controller
>Osmisljeno kao za inhouse projekat za uzgoj biljaka. 
Trenutno zavrsena samo relugacija toplote.

##TODO:
1. Kontrola pumpe za vodu.
2. Kontrola svetla preko releja.
3. Logovanje na SD karticu.

[![Build Status][travis-image]][travis-url]

# Usage
Upload na plocu

Napomena: Senzor temperature nije u povezan kao parazit

# Requirements
Potrebni libovi:
```sh
DallasTemperature
U8g2
OneWire
```
Hardver:
```sh
Senzor za temperaturu: DS18B20
Ekran: OLED 128x32 0.91inch
Relay board
Senzor za vlagu(kapacitet)
```

## Kontribucije
1. Fork (https://github.com/coaalst/Arduino-Temperature-Controller/fork>)
2. Create feature branch (`git checkout -b feature/fooBar`)
3. Commit changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

[travis-image]: https://img.shields.io/travis/dbader/node-datadog-metrics/master.svg?style=flat-square
[travis-url]: https://travis-ci.org/dbader/node-datadog-metrics
