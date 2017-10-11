<html>

<HEAD><title> Buy Saturday With Mez </title> </HEAD> 

<body bgcolor=lightblue>


<?php

$unit_price = $_REQUEST[ "unit_price" ];
$quantity = $_REQUEST[ "quantity" ];
$discount_code = $_REQUEST[ "discount_code" ];

# tax_rate is blank if not a NM state resident
$tax_rate = $_REQUEST[ "tax_rate" ];

$country = $_REQUEST[ "country" ];

$shipping;

$tooManyBooks = 0;

if( strcmp( $country, "US" ) == 0 ) { 
    switch( $quantity ) {
        case 1:
            $shipping = 3.50;
            break;
        case 2:
        case 3:
            $shipping = 4.90;
            break;
        default:
            $tooManyBooks = 1;
            $shipping = -1;
            break;
        }
    }
else {
    // canada
    switch( $quantity ) {
        case 1:
            $shipping = 5.00;
            break;
        case 2:
        case 3:
            $shipping = 11.45;
            break;
        default:
            $tooManyBooks = 1;
            $shipping = -1;
            break;
        }
    }


$code_correct = 0;
$code_incorrect = 0;

if( strcmp( $discount_code, "" ) != 0 ) {
	if( strcmp( $discount_code, "505111998" ) == 0 ) {
		$unit_price = 0.80 * $unit_price;

		$code_correct = 1;
		}
	else {
		$code_incorrect = 1;
		}
}

if( $code_correct ) {
?>
<CENTER>
Discount code applied.  You are paying 
<b>$<?php printf( "%0.2f", $unit_price );?></b> per book.</CENTER><br>

<?php	
    }


if( $code_incorrect ) {
?>
<center>
<b>Incorrect discount code entered.</b>
</center><br>

<?php	
    }



$bookCost = $unit_price * $quantity;

$subTotal = $bookCost + $shipping;

$tax = ( $tax_rate / 100 ) * $subTotal;

$total = $subTotal + $tax;

# format all with 2 decimal places
$shipping = number_format( $shipping, 2, ".", "");
$bookCost = number_format( $bookCost, 2, ".", "");
$tax = number_format( $tax, 2, ".", "");
$total = number_format( $total, 2, ".", "");

$bookPluralSuffix = "";

if( $quantity > 1 ) {
    $bookPluralSuffix = "s";
    }

if( $tooManyBooks ) {

?>

<CENTER>
To find out about shipping for <?php echo $quantity;?> books, please contact me at laurenserafin@yahoo.com.
</CENTER>

<?php

    }
else {

?>

<CENTER>
<TABLE BORDER=0>
<TR>
<TD><?php echo $quantity;?> Book<?php echo $bookPluralSuffix;?>:</TD>
<TD ALIGN=RIGHT>$<?php echo $bookCost;?></TD>
</TR>
<TR>
<TD>Shipping:</TD>
<TD ALIGN=RIGHT>$<?php echo $shipping;?></TD>
</TR>
<TR>
<TD>Tax:</TD>
<TD ALIGN=RIGHT>$<?php echo $tax;?></TD>
</TR>
<TR>
<TD valign=top><B>Total:</B></TD>
<TD ALIGN=RIGHT valign=top><B>$<?php echo $total;?><br>(US dollars)</B></TD>
</TR>
</TABLE>
</CENTER>
<BR>

<BR>

You can pay for your order in two ways:

<p>1) by sending a check*, US money order, or international money order for US $<?php echo $total;?> to:</p>

<p>Lauren Serafin<br>
1941 Calle De Suenos<br>
Las Cruces, NM 88001<br>
USA</p>

<p>* note: books will be sent once checks are cleared.  If you are sending a check in Canadian dollars, please <a href="http://www.xe.com/ucc/convert.cgi?Amount=<?php echo $total;?>&From=USD&To=CAD" target=new>click here</a> to compute the currency conversion.</p>

<p>2) by using your Visa, MasterCard, or PayPal account:</p>

<form action="https://www.paypal.com/cgi-bin/webscr" method="post">
<input type="hidden" name="cmd" value="_xclick">
<input type="hidden" name="business" value="laurenserafin@yahoo.com">
<input type="hidden" name="quantity" value="<?php echo $quantity;?>">
<input type="hidden" name="item_name" value="Saturday with Mez">
<input type="hidden" name="item_number" value="001">
<input type="hidden" name="amount" value="<?php echo $unit_price;?>">
<input type="hidden" name="shipping" value="<?php echo $shipping;?>">
<input type="hidden" name="tax" value="<?php echo $tax;?>">
<input type="hidden" name="cn" value="Please tell us how you heard about our book">
<input type="hidden" name="currency_code" value="USD">
<input type="hidden" name="lc" value="US">
<input type="hidden" name="bn" value="PP-BuyNowBF">
<input type="image" src="https://www.paypal.com/en_US/i/btn/x-click-butcc.gif" border="0" name="submit" alt="Make payments with PayPal - it's fast, free and secure!">
<img alt="" border="0" src="https://www.paypal.com/en_US/i/scr/pixel.gif" width="1" height="1">
</form>



<p>Many people have reported problems using Paypal. If you are having problems with Paypal, send me an email at laurenserafin@yahoo.com. Let me know how many books you want and I can bill you through Paypal by sending you an invoice. </p>

<?php
    }
?>

</BODY>

</HTML>
