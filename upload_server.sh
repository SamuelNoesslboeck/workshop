sudo cp "workshop.conf" "/etc/apache2/sites-available"

# Simple uploading scripts
sudo cp -a "www/." "/var/www/workshop"

sudo chgrp -R www-data /var/www
sudo chmod -R g+w /var/www

sudo a2ensite workshop
sudo systemctl reload apache2
sudo service apache2 restart