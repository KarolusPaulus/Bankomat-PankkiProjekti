# The SimulBank Project

A year 1 group project with the goal was to create a mock ATM. This included reading data from a serial port connected to a card reader, creating the bankomat program, connecting the bankomat to a backend server and connecting that server to a database with user information. As bonuses we also added a Swagger interface for documentation and admin controls, and a netbank component.

## Participants and credits
Everyone was involved in all sections and/or tools of the base product, but specializations and extras include:

[@Nornalite](https://github.com/Nornalite): Backend servers, display of user data, general digestion of what is supposed to be done, netbank.

[@KarolusPaulus](https://github.com/KarolusPaulus): PIN code module and its conversion into a separate library, visuals.

[@nidacnr](https://github.com/nidacnr): Transaction history window and functions.

[@Tappivanukas2](https://github.com/Tappivanukas2): Transaction window and functions.

[@zopuli](https://github.com/zopuli): Reading data from the serial port, UI.

## Tools, languages etc. involved
The frontend was written in C++ and used Qt Creator's UI tools. The backend was Node.js-enabled JavaScript and hosted on Render for 24/7 access. The database utilised MySQL and was put into the cloud service provider Aiven for the final product and ease of access.

## Samples
#### The card reader and "cards" used
![Faux cards](readme_images/bankomat_cards.png?raw=true "Faux cards")

#### Starting window with PIN entry
![PIN request](readme_images/bankomat_pin.png?raw=true "PIN request")

#### Main menu
![Bankomat main page](readme_images/bankomat_menu.png?raw=true "Bankomat main page")

#### User profile window
![Bankomat user details](readme_images/bankomat_user.png?raw=true "Bankomat user details")

#### Snippet from the Swagger page
![Swagger page](readme_images/swagger.png?raw=true "Swagger page")

#### The structure of the system
![General structure](readme_images/structure.png?raw=true "General structure")

#### The structure of the database
![Database structure](readme_images/database.png?raw=true "Database structure")

#### The logic behind the Bankomat
![Bankomat logic graph](readme_images/bankomat_logic.png?raw=true "Bankomat logic graph")

#### Netbank's user profile page
![Netbank user details](readme_images/netbank_profile.png?raw=true "Netbank user details")

#### Youtube demo (in Finnish)
[![Demo](https://img.youtube.com/vi/evrqCWDBZTA/0.jpg)](https://www.youtube.com/watch?v=evrqCWDBZTA)
