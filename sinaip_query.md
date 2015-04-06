# sinaip\_query #

sinaip\_query — Returns the detailed City information found in the SinaIP Database

## Description ##
```
array sinaip_query ( resource $handle, int $long_ip )
```


The **sinaip\_query()** function will return the record information corresponding to an IP address.

## Return Values ##

  * "country" -- The country name
  * "province" -- The Province.
  * "city" -- The city.
  * "district" -- The district.
  * "isp" -- The isp.
  * "type" -- The type.
  * "desc" -- Description.
  * "lat" -- The Latitude as signed double.
  * "lng" -- The Longitude as signed double.